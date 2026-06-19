#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "uart.h"
#include "manager.h"

/* FILE */
#define STOCK_FILE "stock1.csv"

/* STRUCTURE */
typedef struct
{
    char name[30];
    char card[20];
    int qty;
    char unit[15];
    int price;

}Item;

/* ================= DISPLAY STOCK ================= */

void display_manager_stock()
{
    FILE *fp = fopen(STOCK_FILE,"r");

    char line[200];
    Item item;

    if(fp)
    {
        printf("\n========== CURRENT STOCK ==========\n");

        fgets(line,sizeof(line),fp);

        while(fgets(line,sizeof(line),fp))
        {
            sscanf(line,"%[^,],%[^,],%d,%[^,],%d",

            item.name,
            item.card,
            &item.qty,
            item.unit,
            &item.price);

            printf("\nProduct : %s\n",item.name);
            printf("Card ID : %s\n",item.card);
            printf("Qty     : %d %s\n",item.qty,item.unit);
            printf("Price   : %d\n",item.price);
        }

        printf("===================================\n");

        fclose(fp);
    }
}

/* ================= ADD / UPDATE STOCK ================= */

void manager_add_stock()
{
    char name[30];
    char card[20];
    char unit[15];

    int qty;
    int price;

    FILE *fp;
    FILE *temp;

    Item item;

    char line[200];

    int found = 0;

    printf("\nENTER ITEM NAME : ");
    scanf("%s",name);

    printf("ENTER CARD ID : ");
    scanf("%s",card);

    printf("ENTER QUANTITY : ");
    scanf("%d",&qty);

    printf("ENTER UNIT : ");
    scanf("%s",unit);

    printf("ENTER PRICE : ");
    scanf("%d",&price);

    fp = fopen(STOCK_FILE,"r");
    temp = fopen("temp.csv","w");

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
            item.qty += qty;

            strcpy(item.unit,unit);

            item.price = price;

            found = 1;

            printf("\nSTOCK UPDATED SUCCESSFULLY\n");
        }

        fprintf(temp,"%s,%s,%d,%s,%d\n",

        item.name,
        item.card,
        item.qty,
        item.unit,
        item.price);
    }

    if(!found)
    {
        fprintf(temp,"%s,%s,%d,%s,%d\n",

        name,
        card,
        qty,
        unit,
        price);

        printf("\nNEW PRODUCT ADDED\n");
    }

    fclose(fp);
    fclose(temp);

    remove(STOCK_FILE);
    rename("temp.csv",STOCK_FILE);
}

/* ================= DELETE PRODUCT ================= */

void manager_delete_stock()
{
    char delete_card[20];

    FILE *fp;
    FILE *temp;

    Item item;

    char line[200];

    int found = 0,red_qua;

    printf("\nENTER PRODUCT CARD ID TO DELETE : ");
    scanf("%s",delete_card);
    printf("\nEnter the quantity to reduce : ");
    scanf("%d",&red_qua);

    fp = fopen(STOCK_FILE,"r");
    temp = fopen("temp.csv","w");

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

        if(strcmp(delete_card,item.card)==0)
        {

		
            found = 1;
		 
	    if(red_qua>=item.qty)
	    {
		    item.qty=0;
            	printf("\nPRODUCT DELETED\n");
            	printf("Product Name : %s\n",item.name);
	           
	    }
	    else
	    {
		    item.qty=item.qty-red_qua;
		    printf("\n Quantity Update!!\n ");
		    printf("Remaining Quantity:%d\n",item.qty);
	    }

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

    if(!found)
    {
        printf("\nPRODUCT NOT FOUND\n");
    }
}

/* ================= MANAGER MODE ================= */

void manager_menu(int fd)
{
    int choice;

    while(1)
    {
        printf("\n================================\n");
        printf("        MANAGER MENU\n");
        printf("================================\n");

        printf("1. DISPLAY STOCK\n");
        printf("2. ADD / UPDATE STOCK\n");
        printf("3. DELETE PRODUCT\n");
        printf("4. EXIT MANAGER MODE\n");

        printf("\nENTER CHOICE : ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                display_manager_stock();
                break;

            case 2:
                manager_add_stock();
                break;

            case 3:
                manager_delete_stock();
                break;

            case 4:
                printf("\nEXITING MANAGER MODE...\n");
		serialPuts(fd,"DONE$");
         
                return;

            default:
                printf("\nINVALID CHOICE\n");
        }
    }
}
