#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define STOCK_FILE "stock.txt"
#define BANK_FILE "bank.txt"

//read message


void read_msg(char *msg)
{
        char ch;
        int i=0;
        while(1)
        {
                ch =getchar();
                if(ch=='$')
                {
                        msg[i]='\0';
                        break;
                }
                msg[i++]=ch;
        }
}

//find item price


int find_item_price(char *card,int *price)
{
        FILE *fp=fopen(STOCK_FILE,"r");
        char name[20],cno[20];
        int qty,pr;
        while(fscanf(fp,"%s %s %d %d",name,cno,&qty,&pr)!=EOF)
        {
                if(strcmp(card,cno)==0)
                {
                        *price=pr;
                        fclose(fp);
                        return 1;
                }
        }
        fclose(fp);
        return 0;
}

//updat stock

void update_stock(char *card,int delta)
{
        FILE *fp=fopen(STOCK_FILE,"r");
        FILE *temp=fopen("temp.txt","w");
        char name[20],cno[20];
        int qty,price;
        while(fscanf(fp,"%s %s %d %d",name,cno,&qty,&price)!=EOF)
        {
                if(strcmp(card,cno)==0)
                {
                        qty+=delta;
                }

                fprintf(temp,"%s %s %d %d\n",name,cno,qty,price);
        }
        fclose(fp);
        fclose(temp);
        remove(STOCK_FILE);
        rename("temp.txt",STOCK_FILE);

}


//MANAGER MODE 



void manager_add_item()
{
        FILE *fp=fopen(STOCK_FILE,"a");
        char name[20],card[20];
        int qty,price;
        printf("ADD ITEM(name card qty price):\n");
        scanf("%s %s %d %d",name,card,&qty,&price);
        fprintf(fp,"%s %s %d %d\n",name,card,qty,price);
        fclose(fp);
}


//payment

int process_payment(char *card,int amount)
{
        FILE *fp=fopen(BANK_FILE,"r");
        FILE *temp=fopen("temp_bank.txt","w");
        char cno[20];
        int bal,pin,input_pin;
        int found=0;
        printf("ENTER PIN:\n");
        scanf("%d",&input_pin);
        while(fscanf(fp,"%s %d %d",cno,&bal,&pin)!=EOF)
        {
                if(strcmp(card,cno)==0)
                {
                        found=1;
                        if(pin!=input_pin)
                        {


                                fprintf(temp,"%s %d %d\n",cno,bal,pin);
                                fclose(fp);
                                fclose(temp);
                                return 3;//wrong pin
                        }
                        if(bal<amount)
                        {
                                fprintf(temp,"%s %d %d\n",cno,bal,pin);
                                fclose(fp);
                                fclose(temp);
                                return 2;//low balance
                        }
                        bal-=amount;
                }
                fprintf(temp,"%s %d %d\n",cno,bal,pin);
        }
        fclose(fp);
        fclose(temp);
        remove(BANK_FILE);
        rename("temp_bank.txt",BANK_FILE);
        if(!found)
        {
                return 0;
        }
        return 1;
}



// MAIN


int main()
{
        char msg[50];
        char card[20];
        int total=0;
        printf("SMARTCARTREADY\n");
        while(1)
        {
                read_msg(msg);
                /*ADD ITEM*/
                if(msg[0]='C')
                {
                        strcpy(card,&msg[1]);
                        int price;
                        if(find_item_price(card,&price))
                        {
                                update_stock(card,-1);
                                total+=price;
                                printf("%d$\n",price);
                        }

                        else
                        {
                                printf("0$\n");
                        }
                        fflush(stdout);
                }
                /*DELETE ITEM*/
                else if(msg[0]=='D')
                {
                        strcpy(card,&msg[1]);
                        update_stock(card,+1);
                        printf("OK$\n");
                        fflush(stdout);
                }
                /*MANAGER*/
                else if(msg[0]=='M')
                {
                        manager_add_item();
                        printf("OK$\n");
                        fflush(stdout);
                }
 	        /*PAYMENT*/
                else if(msg[0]=='B')
                {
                        char atm[20];
                        int amount;
                        sscanf(msg+1,"%[^:]:%d",atm,&amount);
                        int res=process_payment(atm,amount);
                        if(res==1)
                                printf("SUCCFSS$\n");
                        else if(res==2)
                                printf("LOWBAL$\n");
                        else if(res==3)
                                printf("WRONGPIN$\n");
                        else
                                printf("INVALID$\n");
                        fflush(stdout);
                        total =0;
                }
        }
}
