#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096

//主状态机
enum CHECK_STATE
{
    CHECK_STATE_REQUESTLINE = 0, 	//当前正在分析请求行
    CHECK_STATE_HEADER,				//当前正在分析头部字段
    CHECK_STATE_CONTENT
};

//从状态机，行的读取状态
enum LINE_STATUS
{
    LINE_OK = 0,		//读到完整行
    LINE_BAD,			//行出错
    LINE_OPEN			//行数据不完整
};

//HTTP请求结果
enum HTTP_CODE
{
    NO_REQUEST,			//请求不完整， 需要继续读取客户端数据
    GET_REQUEST,			//获得了完整的客户请求
    BAD_REQUEST,			//客户请求有语法错误
    FORBIDDEN_REQUEST,	//客户对资源没有足够的访问权限
    INTERNAL_ERROR,		//服务器内部错误
    CLOSED_CONNECTION		//客户端已关闭连接
};

LINE_STATUS parse_line(char *buffer, int &checked_index, int &read_index);
HTTP_CODE parse_content(char *buffer, int &checked_index, CHECK_STATE &checkstate, int &read_index, int &start_index);
HTTP_CODE parse_requestline(char *szTemp, CHECK_STATE &checkstate);
HTTP_CODE parse_headers(char *szTemp);

//发给客户端的应答报文（简化版）
static const char *szret[] = {"I get a correct result\n",
                              "Something wrong\n"};

int main(int argc, char **argv)
{
    int port = 20999;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);

    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    int ret = bind(sockfd, (struct sockaddr*) &addr, sizeof(addr));
    assert(ret != -1);

    ret = listen(sockfd, 10);
    assert(ret != -1);

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int fd = accept(sockfd, (struct sockaddr*) &client_addr, &client_addr_len);
    assert(fd >= 0);

    char BUFFER[BUFFER_SIZE];
    memset(BUFFER, '\0', sizeof(BUFFER));

    int data_read = 0;
    int read_index = 0;
    int checked_index = 0;
    int start_line = 0;

    CHECK_STATE checkstate = CHECK_STATE_REQUESTLINE;

    while(true)
    {
        data_read = recv(fd, BUFFER + read_index, BUFFER_SIZE - read_index, 0);
        if(data_read < 0)
        {
            break;
        }
        else if(data_read == 0)
        {
            std::cout << "content null" << std::endl;
            sleep(3);
        }

        read_index += data_read;

        HTTP_CODE result = parse_content(BUFFER, checked_index, checkstate, read_index, start_line);
        if (result == NO_REQUEST) {	//不完整请求
            continue;
        }
        else if (result == GET_REQUEST) {	//完整请求
            send(fd, szret[0], strlen(szret[0]), 0);
            break;
        }
        else {	//其它情况表示发生错误
            send(fd, szret[1], strlen(szret[1]), 0);
            break;
        }
    }

    return 0;
}

HTTP_CODE parse_headers(char *szTemp)
{
    if (szTemp[0] == '\0') {
        return GET_REQUEST;		//遇到空行，表示我们得到一个正确的HTTP请求
    }
    else if (strncasecmp(szTemp, "Host:", 5) == 0) {
        szTemp += 5;
        szTemp += strspn(szTemp, " \t");
        std::cout << "the request host is: " << szTemp << std::endl;
    }
    else {	//其它头部字段不处理
        std::cout << "I can not handle this header" << std::endl;
    }

    return NO_REQUEST;
}

//解析一行内容
LINE_STATUS parse_line(char *buffer, int &checked_index, int &read_index)
{
    char temp;

    for (; checked_index < read_index; ++checked_index) {
        temp = buffer[checked_index];
        if (temp == '\r') {
            if ((checked_index + 1) == read_index) {
                return LINE_OPEN;
            }
            else if (buffer[checked_index+1] == '\n') {
                buffer[checked_index++] = '\0';
                buffer[checked_index++] = '\0';

                return LINE_OK;
            }

            return LINE_BAD;
        }
        else if (temp == '\n') {
            if (checked_index > 1 && buffer[checked_index-1] == '\r') {
                buffer[checked_index-1] = '\0';
                buffer[checked_index++] = '\0';

                return LINE_OK;
            }

            return LINE_BAD;
        }
    }

    return LINE_OPEN;
}

HTTP_CODE parse_requestline(char *szTemp, CHECK_STATE &checkstate)
{
    char *szURL = strpbrk(szTemp, " \t");
    if (!szURL)
        return BAD_REQUEST;
    *szURL++ = '\0';

    char *szMethod = szTemp;		//获取方法
    if (strcasecmp(szMethod, "GET") == 0)
        std::cout << "The request method is GET" << std::endl;
    else
        return BAD_REQUEST;

    szURL += strspn(szURL, " \t");
    char *szVersion = strpbrk(szURL, " \t");
    if (!szVersion)
        return BAD_REQUEST;
    *szVersion++ = '\0';
    szVersion += strspn(szVersion, " \t");		//获取版本
    if (strcasecmp(szVersion, "HTTP/1.1") != 0)
        return BAD_REQUEST;

    if (strncasecmp(szURL, "http://", 7) == 0) {	//获取URL
        szURL += 7;
        szURL = strchr(szURL, '/');
    }

    if (!szURL || szURL[0] != '/')
        return BAD_REQUEST;

    std::cout << "The request URL is: " << szURL << std::endl;

    checkstate = CHECK_STATE_HEADER;

    return NO_REQUEST;
}

HTTP_CODE parse_content(char *buffer, int &checked_index, CHECK_STATE &checkstate, int &read_index, int &start_index)
{
    LINE_STATUS linestatus = LINE_OK;	//记录当前行的读取状态
    HTTP_CODE retcode = NO_REQUEST;		//记录HTTP请求的处理结果

    //从buffer中取出完整行
    while ((linestatus = parse_line(buffer, checked_index, read_index)) == LINE_OK)
    {
        char *szTemp = buffer + start_index;	//start_line是行的起始位置
        start_index = checked_index;				//记录下一行的起始位置

        switch(checkstate) {
            case CHECK_STATE_REQUESTLINE:    //分析请求行
            {
                retcode = parse_requestline(szTemp, checkstate);
                if (retcode == BAD_REQUEST)
                    return BAD_REQUEST;

                break;
            }
            case CHECK_STATE_HEADER:			//分析头部字段
            {
                retcode = parse_headers(szTemp);
                if (retcode == BAD_REQUEST) {
                    return BAD_REQUEST;
                }
                else if(retcode == GET_REQUEST) {
                    return GET_REQUEST;
                }
                break;
            }
            default:
            {
                return INTERNAL_ERROR;
            }
        }
    }

    if (linestatus == LINE_OPEN)		//不完整行，要继续读取客户端数据才能进一步分析
        return NO_REQUEST;
    else
        return BAD_REQUEST;

    return BAD_REQUEST;
}