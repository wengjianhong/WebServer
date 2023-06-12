#include <unistd.h>
#include <getopt.h>

#include "server.hpp"
#include "web_server.hpp"

void print_usage()
{
    printf("Usage: WebServer --port PORT --path PATH\n");
    printf("  --help           Print this message\n");
    printf("  --port PORT      Server port\n");
    printf("  --path PATH      web source directory\n\n");
}

int parse_options(int argc, char **argv, RunParameters &parameters)
{
    int result = 0;
    int option_char = 0;
    int option_index = 0;
    const char *option_string = "d:p:";

    static struct option long_options[] = {
        {"port", required_argument, NULL, 'r'},
        {"path", required_argument, NULL, 'p'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, 0, 0},
    };

    while ((option_char = getopt_long(argc, argv, option_string, long_options, &option_index)) != -1)
    {
        if (option_char == 'r' && optarg != NULL)
        {
            parameters.port = atoi(optarg);
        }
        else if (option_char == 'p' && optarg != NULL)
        {
            strncpy(parameters.path, optarg, MAX_PATH);
        }
        else if (option_char == 'h' || option_char != 0)
        {
            result = 1;
            break;
        }

        // printf("option_char = %c\n", option_char);           // 命令参数，亦即 -a -b -n -r
        // printf("optarg = %s\n", optarg);                     // 参数内容
        // printf("optind = %d\n", optind);                     // 下一个被处理的下标值
        // printf("argv[optind - 1] = %s\n", argv[optind - 1]); // 参数内容
        // printf("option_index = %d\n", option_index);         // 当前打印参数的下标值
        // printf("\n");
    }

    if (parameters.port <= 1024)
    {
        printf("--port must be an integer greater than 1024\n");
        result = 1;
    }

    if (strlen(parameters.path) == 0)
    {
        printf("--path cannot be empty!\n");
        result = 1;
    }

    if (result != 0)
    {
        print_usage();
    }
    return result;
}

int main(int argc, char **argv)
{
    RunParameters parameters;
    if (parse_options(argc, argv, parameters) != 0)
    {
        return 0;
    }

    WebServer server(parameters.port, parameters.path, MAX_CLIENT_SIZE, THREAD_POOL_SIZE);

    int error_no = server.start();
    CHECK_LOG_RETURN(error_no, 0, "server start failed: code = %d\n", error_no);

    while (server.is_running())
    {
        sleep(100);
    }

    return 0;
}
