#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include<fcntl.h>
#include "uart.h"
#include "manager.h"

/* ================= FILES ================= */
#define STOCK_FILE "stock1.csv"
#define BANK_FILE  "bank.csv"
#define SALES_FILE "sales.csv"

/* ================= STRUCTURES ================= */
typedef struct
{
    char name[30];
    char card[20];
    int qty;
    char unit[15];
    int price;

}Item;

typedef struct
{
    char card[20];
    int balance;
    int pin;

}Bank;

/* ================= GLOBAL ================= */

char pending_card[20];
int pending_amount = 0;

/* ================= DISPLAY STOCK ================= */

void display_stock()
{
    FILE *fp = fopen(STOCK_FILE,"r");

    char line[200];
    Item item;

    if(fp)
    {
        printf("\n========== STOCK STATUS ==========\n");

        fgets(line,sizeof(line),fp);

        while(fgets(line,sizeof(line),fp))
        {
            sscanf(line,"%[^,],%[^,],%d,%[^,],%d",
            item.name,
            item.card,
            &item.qty,
            item.unit,
            &item.price);

            printf("%-10s -> %d %s | Price : %d\n",
            item.name,
            item.qty,
            item.unit,
            item.price);
        }

        printf("==================================\n");

        fclose(fp);
    }
}

/* ================= DISPLAY SALES ================= */

void display_sales()
{
    FILE *fp = fopen(SALES_FILE,"r");

    char line[200];

    if(fp)
    {
        printf("\n========== SALES REPORT ==========\n");

        while(fgets(line,sizeof(line),fp))
        {
            printf("%s",line);
        }

        printf("==================================\n");

        fclose(fp);
    }
}

/* ================= UPDATE SALES ================= */

void update_sales(int amt, char *mode)
{
    /*FILE *fp = fopen(SALES_FILE,"a");

    if(fp)
    {
        time_t t;
        struct tm *tm_info;

        time(&t);

        tm_info = localtime(&t);

        fprintf(fp,
        "%02d-%02d-%04d,%02d:%02d,%d,%s\n",

        tm_info->tm_mday,
        tm_info->tm_mon + 1,
        tm_info->tm_year + 1900,

        tm_info->tm_hour,
        tm_info->tm_min,

        amt,
        mode);

        fclose(fp);
    }*/
	 FILE *fp;
        time_t t;
        struct tm *tm_info;
        char date[20];
        char timebuf[20];
        time(&t);
        tm_info=localtime(&t);
        strftime(date,sizeof(date),"%d-%m-%y",tm_info);
        strftime(timebuf,sizeof(timebuf),"%H:%M:%S",tm_info);

        fp=fopen("sales.csv","a");
        if(fp)
        {
                fprintf(fp,"date:%s  ,time:%s  ,amount:%d\n",date,timebuf,amt);
                fclose(fp);
        }

}

/* ================= ADD ITEM ================= */

int add_item(char *card, int *price, int *qty_out)
{
    FILE *fp = fopen(STOCK_FILE,"r");
    FILE *temp = fopen("temp.csv","w");

    if(!fp || !temp)
        return 0;

    char line[200];
    Item item;

    int found = 0;

    fprintf(temp,"name,card,qty,unit,price\n");

    fgets(line,sizeof(line),fp);

    while(fgets(line,sizeof(line),fp))
    {
        sscanf(line,"%[^,],%[^,],%d,%[^,],%d",

        item.name,
        item.card,
        &item.qty,
        item.unit,
        &item.price);

        if(strcmp(card,item.card)==0 && item.qty > 0)
        {
            printf("\n========== ITEM ADDED =========\n");
            printf("Product Name  : %s\n",item.name);
            printf("RFID Card     : %s\n",item.card);
            printf("Price         : %d\n",item.price);
            printf("Remaining Qty : %d %s\n",
            item.qty-1,
            item.unit);
            printf("================================\n");

            item.qty--;

            *price = item.price;
            *qty_out = item.qty;

            found = 1;
        }

        fprintf(temp,"%s,%s,%d,%s,%d\n",

        item.name,
        item.card,
        item.qty,
        item.unit,
        item.price);
    }

    fclose(fp);
    fclose(temp);

    remove(STOCK_FILE);
    rename("temp.csv",STOCK_FILE);

    return found;
}

/* ================= DELETE ITEM ================= */

int delete_item(char *card)
{
    FILE *fp = fopen(STOCK_FILE,"r");
    FILE *temp = fopen("temp.csv","w");

    if(!fp || !temp)
        return 0;

    char line[200];
    Item item;

    int found = 0;

    fprintf(temp,"name,card,qty,unit,price\n");

    fgets(line,sizeof(line),fp);

    while(fgets(line,sizeof(line),fp))
    {
        sscanf(line,"%[^,],%[^,],%d,%[^,],%d",

        item.name,
        item.card,
        &item.qty,
        item.unit,
        &item.price);

        if(strcmp(card,item.card)==0)
        {
            printf("\n========== ITEM DELETED =========\n");
            printf("Product Name : %s\n",item.name);
            printf("RFID Card    : %s\n",item.card);
            printf("Old Qty      : %d %s\n",
            item.qty,
            item.unit);

            item.qty++;

            printf("Updated Qty  : %d %s\n",
            item.qty,
            item.unit);

            printf("DELETE SUCCESSFUL\n");
            printf("=================================\n");

            found = 1;
        }

        fprintf(temp,"%s,%s,%d,%s,%d\n",
        item.name,
        item.card,
        item.qty,
        item.unit,
        item.price);
    }

    fclose(fp);
    fclose(temp);

    remove(STOCK_FILE);
    rename("temp.csv",STOCK_FILE);

    return found;
}

/* ================= PAYMENT PROCESS ================= */

int process_pin(char *card, int amount, int pin_input, int mode)
{
    FILE *fp = fopen(BANK_FILE,"r");
    FILE *temp = fopen("temp_bank.csv","w");

    if(!fp || !temp)
        return 0;

    char line[100];
    Bank user;

    int found = 0;

    fprintf(temp,"card,balance,pin\n");

    fgets(line,sizeof(line),fp);

    while(fgets(line,sizeof(line),fp))
    {
        sscanf(line,"%[^,],%d,%d",

        user.card,
        &user.balance,
        &user.pin);

        if(strcmp(card,user.card)==0)
        {
            found = 1;

            if(mode == 1)
            {
                fprintf(temp,"%s,%d,%d\n",
                user.card,
                user.balance,
                user.pin);

                continue;
            }

            if(user.pin != pin_input)
            {
                printf("\nWRONG PIN ENTERED\n");

                fprintf(temp,"%s,%d,%d\n",
                user.card,
                user.balance,
                user.pin);

                fclose(fp);
                fclose(temp);

                return 3;
            }

            if(user.balance < amount)
            {
                printf("\nLOW BALANCE\n");

                fprintf(temp,"%s,%d,%d\n",
                user.card,
                user.balance,
                user.pin);

                fclose(fp);
                fclose(temp);

                return 2;
            }

            user.balance -= amount;
        }

        fprintf(temp,"%s,%d,%d\n",

        user.card,
        user.balance,
        user.pin);
    }

    fclose(fp);
    fclose(temp);

    remove(BANK_FILE);
    rename("temp_bank.csv",BANK_FILE);

    if(!found)
    {
        printf("\nINVALID CARD DETECTED\n");
        return 0;
    }

    return 1;
}

/* ================= MAIN ================= */

int main()
{
    int fd;
    int i;

    char ch;
    char buffer[100];

    fd = serialOpen("/dev/ttyUSB0",9600);

    if(fd < 0)
    {
        printf("UART ERROR\n");
        return -1;
    }

    printf("\n=====================================\n");
    printf(" RFID SMART CART BILLING SYSTEM\n");
    printf(" Backend Server Started Successfully\n");
    printf(" UART Communication Active\n");
    printf("=====================================\n");

    while(1)
    {
        memset(buffer,'\0',sizeof(buffer));

        i = 0;

        display_stock();

        while(1)
        {
            ch = serialGetchar(fd);

            if(ch == '$')
            {
                buffer[i] = '\0';
                break;
            }

            if(i < 99)
            {
                buffer[i++] = ch;
            }

            usleep(100);
        }

        printf("\nReceived Data : %s\n",buffer);

        /* ================= ADD ITEM ================= */

        if(buffer[0]=='C')
        {
            char card[20];

            int price = 0;
            int qty = 0;

            strcpy(card,buffer+1);

            if(add_item(card,&price,&qty))
            {
                dprintf(fd,"%d:%d$",price,qty);
            }
            else
            {
                dprintf(fd,"0:0$");
            }
        }

        /* ================= DELETE ITEM ================= */

        else if(buffer[0]=='D')
        {
            char card[20];

            strcpy(card,buffer+1);
            if(delete_item(card))
            {
                dprintf(fd,"DOK$");
            }
            else
            {
                dprintf(fd,"DNF$");
            }
        }

        /* ================= MANAGER MODE ================= */

        else if(strcmp(buffer,"ADMIN")==0)
        {
            manager_menu(fd);
        }

        /* ================= CARD VALIDATION ================= */

        else if(buffer[0]=='B')
        {
            int pin;

            sscanf(buffer+1,"%[^:]:%d",
            pending_card,
            &pending_amount);

            int res = process_pin(
            pending_card,
            pending_amount,
            pin,
            1);

            if(res==1)
            {
                dprintf(fd,"ENTERPIN$");
            }
            else
            {
                dprintf(fd,"INVALID$");
            }
        }

        /* ================= PIN PROCESS ================= */

        else if(buffer[0]=='P')
        {
            int pin;

            sscanf(buffer+1,"%d",&pin);

            int res = process_pin(
            pending_card,
            pending_amount,
            pin,
            2);

            if(res==1)
            {
                update_sales(pending_amount,"CARD");

                printf("\n=================================\n");
                printf("      PAYMENT SUCCESSFUL\n");
                printf("=================================\n");

                printf("Card Number : %s\n",pending_card);
                printf("Amount Paid : %d\n",pending_amount);

                printf("=================================\n");

                display_sales();

                dprintf(fd,"SUCCESS$");
            }

            else if(res==2)
            {
                dprintf(fd,"LOWBAL$");
            }

            else if(res==3)
            {
                dprintf(fd,"WRONGPIN$");
            }

            else
            {
                dprintf(fd,"INVALID$");
            }
        }
    }

    serialClose(fd);

    return 0;
}
