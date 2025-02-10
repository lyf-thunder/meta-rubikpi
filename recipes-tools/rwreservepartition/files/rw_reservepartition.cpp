#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#define SERIALNO_LEN 64
#define CIPHERTEXT_LEN 100
#define CIPHERTEXT_NUM 20
#define MAC_SIZE 16
#define NUM_LEN 99  // the array subscript show the len of the string
#define NUM_FLAG 98  // the array subscript show the positive and negative of the string
#define PI_SERIALNO_LEN 64
#define MAC_SIZE 16
#define TAG "reservepartition"
#define MAC_TOTAL_LENTH 17 /*the mac address format like 28:d2:44:76:43:78,so the total lenth is 17 */
#define PARTITION "sdd1"

typedef struct {
    char serialno[SERIALNO_LEN];
    int serialno_tag;
    char wifimac[MAC_SIZE];
    int wifimac_tag;
    char btmac[MAC_SIZE];
    int btmac_tag;
    char eth0mac[MAC_SIZE];
    int eth0mac_tag;
    char eth1mac[MAC_SIZE];
    int eth1mac_tag;
    char piserialno[PI_SERIALNO_LEN];
    int piserialno_tag;
    int ciphertext[CIPHERTEXT_NUM][CIPHERTEXT_LEN];
    int ciphertext_tag;
}reserveinfo;

// Add supported type
typedef enum {
    SERIALNO,
    WIFIMAC,
    BTMAC,
    ETH0MAC,
    ETH1MAC,
    PISERIALNO,
    OTHER
}RESERVETYPE;

int get_len_from_string(char *ch) {
    int len = 0;

    while ('\0' != *ch) {
        if ('x' == *ch)
           len++;
        ch++;
    }

    return len;
}

int char2int(char ch) {
    return ch - '0';
}

void value_exchange(int value[CIPHERTEXT_LEN]) {
    int i = 0;
    int buf[CIPHERTEXT_LEN];
    int buf_ex[CIPHERTEXT_LEN];

    memset(buf_ex, 0, sizeof(int)*CIPHERTEXT_LEN);
    memcpy(buf, value, sizeof(int)*CIPHERTEXT_LEN);

    for (i = 0; i < buf[NUM_LEN]; i++) {
        buf_ex[i] = buf[buf[NUM_LEN]-i-1];
    }

    buf_ex[NUM_LEN] = value[NUM_LEN];

    memcpy(value, buf_ex, sizeof(int)*CIPHERTEXT_LEN);
}

//  process the ciphertext, every 'x' represent a num string
int ciphertext_string_process(int len, char *ciphertext, int (*my_ciphertext)[CIPHERTEXT_LEN]) {
    int i = 0, j = 0, k = 0;
    char buf[CIPHERTEXT_LEN] = {};
    int (*ciphertext_buf)[CIPHERTEXT_LEN];

    ciphertext_buf = reinterpret_cast<int (*)[CIPHERTEXT_LEN]>
                    (malloc(sizeof(int) * CIPHERTEXT_LEN * CIPHERTEXT_NUM));
    if (NULL == ciphertext_buf) {
      printf("malloc failed\n");
      return -1;
    }

    while (len--) {
        i = 0;
        while (1) {
            buf[i] = *ciphertext;
            i++;
            ciphertext++;
            if ('x' == *ciphertext) {
                ciphertext++;
                break;
            }
        }
        ciphertext_buf[k][NUM_LEN] = i;
        for (j = 0; j < i; j++) {
            ciphertext_buf[k][j] = char2int(buf[j]);
        }
        value_exchange(ciphertext_buf[k]);
        ciphertext_buf[k][NUM_FLAG] = '1';
        k++;
    }
    memcpy(my_ciphertext, ciphertext_buf,
            sizeof(int) * CIPHERTEXT_LEN * CIPHERTEXT_NUM);
    free(ciphertext_buf);
    return 0;
}

const char* getMountBlockForPartition(const std::string& partition) {
    std::string command = "ls -l /dev/" + partition;
    std::string result;
    FILE* pipe = popen(command.c_str(), "r");
    char buffer[128];

    if (pipe == nullptr) {
        printf("Error executing command\n");
        return "";
    }

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);

    // Extract the block device path from the result
    size_t start = result.find_last_of(" -> ");
    if (start != std::string::npos) {
      std::string blockPath = result.substr(start + 1, result.length() - start - 2);
      const char* blockPathStr = blockPath.c_str();
      // printf("blockPathStr%s \n", blockPathStr);
      return blockPathStr;
    }

    return "";
}

int write_reserve_partition(char *ptype, int rw, void *data) {
    int fd = -1;
    int ret = -1;
    int type = -1;
    reserveinfo *reserve_buf;
    reserveinfo *check_reserve_buf;

    reserve_buf = reinterpret_cast<reserveinfo *>(malloc(sizeof(reserveinfo)));
    if (NULL == reserve_buf) {
      printf("malloc failed\n");
      return -1;
    }
    check_reserve_buf = reinterpret_cast<reserveinfo *>(malloc(sizeof(reserveinfo)));
    if (NULL == check_reserve_buf) {
      printf("malloc failed\n");
      free(reserve_buf);
      return -1;
    }

    if (NULL == ptype || (1 == rw && NULL == data)) {
      free(reserve_buf);
      free(check_reserve_buf);
      return -1;
    }
    // Add supported type
    if (!strcmp(ptype, "serialno")) {
      type = SERIALNO;
    } else if (!strcmp(ptype, "wifimac")) {
      type = WIFIMAC;
    } else if (!strcmp(ptype, "btmac")) {
      type = BTMAC;
    } else if (!strcmp(ptype, "eth0mac")) {
      type = ETH0MAC;
    } else if (!strcmp(ptype, "eth1mac")) {
      type = ETH1MAC;
    } else if (!strcmp(ptype, "piserialno")) {
      type = PISERIALNO;
    } else {
      printf("Please input right type %s \n", ptype);
      free(reserve_buf);
      free(check_reserve_buf);
      return -1;
    }

    if ((fd = open(getMountBlockForPartition(PARTITION), O_RDWR)) < 0) {
      printf("open %s failed \n", PARTITION);
      free(reserve_buf);
      free(check_reserve_buf);
      return -1;
    }

    if (sizeof(reserveinfo) == read(fd, reserve_buf, sizeof(reserveinfo))) {
      switch (type) {
        case SERIALNO:
          // printf("read serialno %s \n", reserve_buf->serialno);
          if (1 == rw) {
            snprintf(reserve_buf->serialno, SERIALNO_LEN, "%s", reinterpret_cast<char *>(data));
            reserve_buf->serialno_tag = 1;
          }
          break;

        case WIFIMAC:
          // printf("read wifi mac %s \n", reserve_buf->wifimac);
          if (1 == rw) {
            if ((MAC_SIZE - 4) == strlen((const char *)data)) {
              snprintf(reserve_buf->wifimac, MAC_SIZE, "%s", reinterpret_cast<char *>(data));
              reserve_buf->wifimac_tag = 1;
            } else {
              printf("write wifi mac %s failed\n", (char *)data);
            }
          }
          break;

        case BTMAC:
          // printf("read bt mac %s \n", reserve_buf->btmac);

          if (1 == rw) {
            if ((MAC_SIZE - 4) == strlen((const char *)data)) {
              snprintf(reserve_buf->btmac, MAC_SIZE, "%s", reinterpret_cast<char *>(data));
              reserve_buf->btmac_tag = 1;
            } else {
              printf("write bt mac %s failed\n", (char *)data);
            }
          }
          break;

        case ETH0MAC:
          // printf("read eth0 mac %s \n", reserve_buf->eth0mac);
          if (1 == rw) {
            if ((MAC_SIZE - 4) == strlen((const char *)data)) {
              snprintf(reserve_buf->eth0mac, MAC_SIZE, "%s", reinterpret_cast<char *>(data));
              reserve_buf->eth0mac_tag = 1;
            } else {
              printf("write eth0 mac %s failed\n", (char *)data);
            }
          }
          break;

        case ETH1MAC:
          // printf("read eth1 mac %s \n", reserve_buf->eth1mac);
          if (1 == rw) {
            if ((MAC_SIZE - 4) == strlen((const char *)data)) {
              snprintf(reserve_buf->eth1mac, MAC_SIZE, "%s", reinterpret_cast<char *>(data));
              reserve_buf->eth1mac_tag = 1;
            } else {
              printf("write eth1 mac %s failed\n", (char *)data);
            }
          }
          break;
        
          case PISERIALNO:
          // printf("read serialno %s \n", reserve_buf->serialno);
          if (1 == rw) {
            snprintf(reserve_buf->piserialno, PI_SERIALNO_LEN, "%s", reinterpret_cast<char *>(data));
            reserve_buf->piserialno_tag = 1;
          }
          break;

        case OTHER:
          break;

        default:
          break;
      }
    }

    if (lseek(fd, 0, SEEK_SET) < 0) {
      printf("lseek failed \n");
      goto operation_err;
    }

    if (1 == rw && sizeof(reserveinfo) == write(fd, reserve_buf, sizeof(reserveinfo))) {
      // printf("write serialno %s \n", reserve_buf->serialno);
      ret = 0;
    }

    lseek(fd, 0, SEEK_SET);
    ret = read(fd, check_reserve_buf, sizeof(reserveinfo));
    if (ret < 0) {
      printf("check read failed\n");
      goto operation_err;
    }

    if (memcmp(check_reserve_buf, reserve_buf, sizeof(reserveinfo))) {
      printf("write failed\n");
      goto operation_err;
    }
    // printf("write success\n");

    close(fd);
    free(reserve_buf);
    free(check_reserve_buf);
    return ret;

operation_err:
    close(fd);
    free(reserve_buf);
    free(check_reserve_buf);
    return -1;
}

void ethernet_mac_format(char *mac_addr) {
    int i = 0;
    char out_buff[MAC_TOTAL_LENTH+1] = {};

    for ( i = 0; i <= MAC_TOTAL_LENTH; i++ ) {
        if (0 == ((i+1)%3)) {
            out_buff[i] = ':';
        } else {
            out_buff[i] = *mac_addr;
            mac_addr++;
        }
        out_buff[MAC_TOTAL_LENTH] = '\0';
    }
    printf("%s\n", out_buff);
}


int read_reserve_partition(char *ptype, int rw, void *data) {
    int fd = -1;
    int type = -1;
    reserveinfo *reserve_buf;

    reserve_buf = reinterpret_cast<reserveinfo *>(malloc(sizeof(reserveinfo)));
    if (NULL == reserve_buf) {
      printf("malloc failed\n");
      return -1;
    }

    if (NULL == ptype || (1 == rw && NULL == data)) {
      free(reserve_buf);
      return -1;
    }
    // Add supported type
    if (!strcmp(ptype, "serialno")) {
      type = SERIALNO;
    } else if (!strcmp(ptype, "wifimac")) {
      type = WIFIMAC;
    } else if (!strcmp(ptype, "btmac")) {
      type = BTMAC;
    } else if (!strcmp(ptype, "eth0mac")) {
      type = ETH0MAC;
    } else if (!strcmp(ptype, "eth1mac")) {
      type = ETH1MAC;
    } else if (!strcmp(ptype, "piserialno")) {
      type = PISERIALNO;
    } else {
      printf("Please input right type %s \n", ptype);
      free(reserve_buf);
      return -1;
    }
    // printf("read_reserve_partition type =%s \n", ptype);
    if ((fd = open(getMountBlockForPartition(PARTITION), O_RDWR)) < 0) {
      printf("open %s failed \n", PARTITION);
      free(reserve_buf);
      return -1;
    }

    if (sizeof(reserveinfo) == read(fd, reserve_buf, sizeof(reserveinfo))) {
      switch (type) {
        case SERIALNO:
          printf("%s\n", reserve_buf->serialno);
          break;

        case WIFIMAC:
          printf("%s\n", reserve_buf->wifimac);
          break;

        case BTMAC:
          printf("%s\n", reserve_buf->btmac);
          break;

        case ETH0MAC:
         ethernet_mac_format(reserve_buf->eth0mac);
          break;

        case ETH1MAC:
          ethernet_mac_format(reserve_buf->eth1mac);
          break;
        
        case PISERIALNO:
          printf("%s\n", reserve_buf->piserialno);
          break;
        
        case OTHER:
          break;

        default:
         break;
      }
    }
    free(reserve_buf);
    close(fd);
    return 0;
}

int write_ciphertext_to_partition(char *ciphertext) {
  int fd = 0;
  int ret = 0;
  int len = 0;
  reserveinfo *buf;
  reserveinfo *check_buf;

  buf = reinterpret_cast<reserveinfo *>(malloc(sizeof(reserveinfo)));
  if (NULL == buf) {
    printf("malloc failed\n");
    return -1;
  }

  check_buf = reinterpret_cast<reserveinfo *>(malloc(sizeof(reserveinfo)));
  if (NULL == check_buf) {
    printf("malloc failed\n");
    free(buf);
    return -1;
  }

  len = get_len_from_string(ciphertext);
  if (0 == len) {
      printf("invaild ciphertext");
      free(buf);
      free(check_buf);
      return -1;
  }

  fd = open(getMountBlockForPartition(PARTITION), O_RDWR);
  if (fd < 0) {
      printf("open %s failed\n", PARTITION);
      free(buf);
      free(check_buf);
      return -1;
  }

  lseek(fd, 0, SEEK_SET);
  ret = read(fd, buf, sizeof(reserveinfo));
  if (ret < 0) {
      printf("read %s failed\n", PARTITION);
      goto operation_err;
  }
  ret = ciphertext_string_process(len, ciphertext, buf->ciphertext);
  if (ret < 0) {
    printf("process string failed\n");
    goto operation_err;
  }

  lseek(fd, 0, SEEK_SET);
  buf->ciphertext_tag = 1;
  ret = write(fd, buf, sizeof(reserveinfo));
  if (ret < 0) {
      printf("Write failed\n");
      goto operation_err;
  }

  lseek(fd, 0, SEEK_SET);
  ret = read(fd, check_buf, sizeof(reserveinfo));
  if (ret < 0) {
      printf("Read failed\n");
      goto operation_err;
  }

  if (memcmp(check_buf, buf, sizeof(reserveinfo))) {
    printf("Write failed");
    goto operation_err;
  }

  // printf("Write success\n");

  free(check_buf);
  free(buf);
  close(fd);

  return 0;

operation_err:
  free(check_buf);
  free(buf);
  close(fd);

  return -1;
}

// rw_reservepartition r/w type content
// For example:
// rw_reservepartition r serialno //For read
// rw_reservepartition w serialno  content //For write
int main(int argc, char *argv[]) {
    int ret = 0;
    if (argc < 3) {
      printf("Please input right parameters\n");
    }
    if (3 == argc && 'r' == argv[1][0]) {
      ret = read_reserve_partition(argv[2], 0, NULL);
      if (ret < 0) {
        return -1;
      } else {
        return 0;
      }
    } else if (4 == argc && 'w' == argv[1][0]) {
      // Set serialno to lun3
      if (strlen(argv[3]) <= SERIALNO_LEN) {
        ret = write_reserve_partition(argv[2], 1, argv[3]);
        if (ret < 0) {
          return -1;
        } else {
          return 0;
        }
      }
    } else if (3 == argc && 's' == argv[1][0]) {
        ret = write_ciphertext_to_partition(argv[2]);
        if (ret < 0) {
           return -1;
        } else {
           return 0;
        }
    } else {
      printf("Please input right parameters \n");
    }
    return 0;
}

