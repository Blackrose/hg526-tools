#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUF_SIZE 1024
#define PRO_NAME "HG526-cfg-DEncoder"

static void usage(void);
static void decode(int *src_fd, int *trg_fd);
static void encode(int *src_fd, int *trg_fd);

int main(int argc, char *argv[])
{
    int srcfd, trgfd;
    int ch;
    char src_filename[256], trg_filename[256];
    int decode_flg = 0, encode_flg = 0;

    if(argc < 5)
    {
        usage();
        exit(0);
    }


    memset(src_filename, 0, sizeof(src_filename));
    memset(trg_filename, 0, sizeof(trg_filename));

    while((ch =getopt(argc, argv, "DEi:o:")) != -1)
    {
        switch(ch)
        {
            case 'D':
                decode_flg = 1;
                printf("D is enable\n");
                break;
            case 'E':
                encode_flg = 1;
                printf("E is enable\n");
                break;
            case 'i':
                memcpy(src_filename, optarg, strlen(optarg));
                printf("input file=%s\n", src_filename);
                break;
            case 'o':
                memcpy(trg_filename, optarg, strlen(optarg));
                printf("out file=%s\n", trg_filename);
                break;
            default:
                usage();
                break;

        }
    }


    srcfd = open(src_filename, O_RDONLY);
    if(srcfd < 0)
    {
        perror("open source file error");
        exit(-1);
    }

    trgfd = open(trg_filename, O_CREAT|O_WRONLY, 0666);
    if(trgfd < 0)
    {
        perror("open target file error");
        exit(-2);
    }


    if(decode_flg)
        decode(&srcfd, &trgfd);
    else if(encode_flg)
        encode(&srcfd, &trgfd);

    close(srcfd);
    close(trgfd);

    return 0;
}


static void usage(void)
{
    printf("Usage: %s -D/E -i src_filename -o trg_filename\n", PRO_NAME);
    printf("Get more help, please visit http://blackrose.me\n");
}



static void decode(int *src_fd, int *trg_fd)
{   
    int rfd;
    char src[BUF_SIZE], trg[BUF_SIZE];
    char tmp;
    int i, len;

    printf("Decode file beginning....\n");

    while(1)
    {
        memset(src, 0, sizeof(src));
        
        rfd = read(*src_fd, src, sizeof(src)-1);
        
        if(rfd == 0)
            goto END;
        
        len = strlen(src);
        
        memset(trg, 0, sizeof(trg));
         
        for(i = 0;i < len; i++)
        {
            if(src[i]%2 == 0)
                tmp = src[i]/2;
            else
                tmp = (src[i]+127)/2;

            trg[i] = tmp;
            //printf("%c", trg[i]);
    
        }
        write(*trg_fd, trg, rfd);
    }
END:
    printf("Done\n");
    return;

}

static void encode(int *src_fd, int *trg_fd)
{   
    int rfd;
    char src[BUF_SIZE], trg[BUF_SIZE];
    char tmp;
    int i, len;

    printf("Encode file beginning....\n");

    while(1)
    {
        memset(src, 0, sizeof(src));
        
        rfd = read(*src_fd, src, sizeof(src)-1);
        
        if(rfd == 0)
            goto END;
        
        len = strlen(src);
        
        memset(trg, 0, sizeof(trg));
         
        for(i = 0;i < len; i++)
        {
            if(src[i]*2 > 127)
                tmp = src[i]*2 - 127;
            else
                tmp = src[i]*2;

            trg[i] = tmp;
            //printf("%c", trg[i]);
    
        }
        write(*trg_fd, trg, rfd);
    }
END:
    printf("Done\n");
    return;

}
