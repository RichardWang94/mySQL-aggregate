//#ifndef _mysql_h
//#define _mysql_h



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <my_global.h>

#include <gmp.h>
#include "paillier.h"
#include <mysql.h>

int main(){
	int debug = 1; //if(debug==1) 
	//char word[1024];
	char *word = malloc(1024);//NULL; //for getline
	size_t size;
	char *temp;

//mysql portion
	MYSQL *conn = mysql_init(NULL);
	MYSQL_RES *res;
	//MYSQL_ROW row;
	MYSQL_ROW row = malloc(1024);
	char *server = "54.67.45.42";
	//char *server = "172.30.1.108";

	char *user = "root";
	//char *password = "richardspencer"; // set me first 
	char *password = "cs174$";
	//char *password = "cs174$";
	char *database = "project";
	 
printf("Connection Attempt\n");
	// if (!mysql_real_connect(conn, server,
    //     user, password, database, 0, NULL, 0)) 
	
	 if (!mysql_real_connect(conn, server,
         user, password, database, 3306, NULL, 0)) 
		 {
     	 fprintf(stderr, "%s\n", mysql_error(conn));
     	 exit(1);
		}
	else {printf("Connected!\n");}
	
	
	//queries "show tables" and sends query
	if (mysql_query(conn, "show tables")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
		}
	//catches results (table names) from server and prints
	//row by row result set retrieval
	res = mysql_use_result(conn);	
	printf("MySQL Tables in mysql database:\n");
	while ((row = mysql_fetch_row(res)) != NULL)
		{printf("%s \n", row[0]);}
	mysql_free_result(res);
	mysql_more_results(conn);

	
	//Encryption/Decryption
	//PUB AND PRIVATEKEYS are hardcoded
	paillier_pubkey_t *pubkey;
 	pubkey = (paillier_pubkey_t*)malloc(sizeof(paillier_pubkey_t));
	paillier_prvkey_t *prvkey;
	prvkey = (paillier_prvkey_t*)malloc(sizeof(paillier_prvkey_t));	
	
	//Hardcode public key
	mpz_init(pubkey->n_squared);
	mpz_init(pubkey->n);
	mpz_init(pubkey->n_plusone);
	char public_key[1024] = "30217388200631950129090459934004151318189944848301345317254019538698525090449";
	mpz_t integ;
	mpz_init(integ);
	mpz_set_str(integ, public_key, 10);
	mpz_set(pubkey->n_squared, integ);
	char n[1024] = "173831493696142271079948667154505245143";
	mpz_t integ1;
	mpz_init(integ1);
	mpz_set_str(integ1, n, 10);
	mpz_set(pubkey->n, integ1);
	char np1[1024] = "173831493696142271079948667154505245144";
	mpz_t integ2;
	mpz_init(integ2);
	mpz_set_str(integ2, np1, 10);
	mpz_set(pubkey->n_plusone, integ2);
	pubkey->bits = 128;

	//hardcode private key
	mpz_init(prvkey->lambda);
	mpz_init(prvkey->x);
	char private_key[1024] = "86915746848071135526274426612887025760";
	mpz_t integ3;
	mpz_init(integ3);
	mpz_set_str(integ3, private_key, 10);
	mpz_set(prvkey->lambda, integ3);
	char x[1024] = "94055792145174338086140629840906051804";
	mpz_t integ4;
	mpz_init(integ4);
	mpz_set_str(integ4, x, 10);
	mpz_set(prvkey->x, integ4);
	
	
	// printf("select count(id) from Employees\n");
			// if (mysql_query(conn, "select age, avg(salary) from Employees group by age")) {
			// fprintf(stderr, "%s\n", mysql_error(conn));
			// }
			// res = mysql_use_result(conn);
		
			// //Check for number of columns (should be 3)
			// int fields = mysql_field_count(conn);
			// printf("	number of fields: %i\n",fields);
	// while ((row = mysql_fetch_row(res)) != NULL)
				// { printf("Row: %s %s\n", row[0], row[1]);
					// //printf("%s \n", row[0]);
				// }
				// mysql_free_result(res);
				// mysql_more_results(conn);
	
	
while(1==1){ //loop until exit
	printf("Input command:\n");
	//if 1 argument that isn't exit, ask for proper input using else at end
	//if/else switches on 1st argument (eg select, insert etc)

	getline(&word,&size,stdin);	
	
	
	//separates input into separate tokens and stores each in wordArray
	//wordArray is an array of character pointers; basically, each array member is a word
	//i keeps track of # of arguments
	int i = 0;
	char *wordArray[30]; 
	wordArray[i] = strtok(word, " ");
	while(wordArray[i]!=NULL){	
		//printf("%s\n",wordArray[i]);
		wordArray[++i] = strtok(NULL, " ");
		//i++;
	}
	//printf("# of arguments: %i\n", i);	

	//removes \n from last argument
	wordArray[i-1][strlen(wordArray[i-1])-1]=0;
	
	//Initialize tokens that will accept arguments
	//char *wA0 = malloc(50); 
	char *wA0 =  malloc(50); 
	char *wA1 =  malloc(50); 
	char *wA2 =  malloc(50); 
	char *wA3 =  malloc(1024); 
	char *wA4 =  malloc(1024); 
	//used to form query strings
	//comma space: in between values for insertion
	char *space = malloc(10);
	strcpy(space, ", ");
	//just space: in between select sum arguments
	char *spaceOnly = malloc(10);
	strcpy(spaceOnly, " ");
	//semicolon
	char *semi = malloc(10);
	strcpy(semi, ";");
	
	// //Create string for insert query. Will be concated later on to form proper queries
	// char *insertQuery = malloc(100);
	// strcpy(insertQuery, "INSERT INTO Employees (id, age, salary) values (");
	// char *insertQueryEnd = malloc(10);
	// strcpy(insertQueryEnd, ");");
	
	//Create string for select single query. Will be concated later on to form proper queries
	char *selectQuery = malloc(100);//(char *)malloc(sizeof(char));
	strcpy(selectQuery, "SELECT * FROM Employees WHERE id=");
	char *selectQueryEnd = malloc(10);
	strcpy(selectQueryEnd, ";");
	
	//stores first argument (select, insert etc) into wA0
	strcpy(wA0, wordArray[0]);

	//second token
	//if i (number of arguments) is more than 1, store second argument into wA1
	if(i>1)
		{
		//printf("second token exists\n");
		strcpy(wA1, wordArray[1]);
		}

//EXIT command	
	if((wA0[0]=='e' || wA0[0]=='E') && (wA0[1]=='x' || wA0[1]=='X') 
	&& (wA0[2]=='i' || wA0[2]=='I') && (wA0[3]=='t' || wA0[3]=='T') && (wA0[4]=='\0'))
		{
			printf("Exiting.\n");
			//close mysql connection
			mysql_close(conn);
			printf("Disconnected\n");
			exit(1);
		}
		
//SELECT
	else if((wA0[0]=='s' || wA0[0]=='S') && (wA0[1]=='e' || wA0[1]=='E')
	&& (wA0[2]=='l' || wA0[2]=='L') && (wA0[3]=='e' || wA0[3]=='E') 
	&& (wA0[4]=='c' || wA0[4]=='C') && (wA0[5]=='t' || wA0[5]=='T') && (wA0[6]=='\0'))
		{
	//SELECT ALL: if matches select *		
		if(wA1[0]=='*'&&wA1[1]=='\0')
		{//printf("ALL\n");
			printf("SELECT * from Employees\n");
			if (mysql_query(conn, "select * from Employees;")) 
			{
				fprintf(stderr, "%s\n", mysql_error(conn));
			}
			res = mysql_use_result(conn);
			
			//Check for number of columns (should be 3)
			// int fields = mysql_field_count(conn);
			// printf("	number of fields: %i\n",fields);
			
			//Prints contents of entire table
			printf("Employees table rows:\n");
			while ((row = mysql_fetch_row(res)) != NULL)
			{ 
				//convert row[2] from ciphertext to string
				paillier_plaintext_t *plaintext; 
				paillier_ciphertext_t *ciphertext;
				plaintext= (paillier_plaintext_t*)malloc(sizeof(paillier_plaintext_t));
				ciphertext= (paillier_ciphertext_t*)malloc(sizeof(paillier_ciphertext_t));
				mpz_init(plaintext->m);

				int val;	
				val=mpz_init_set_str(ciphertext->c, row[2], 10);
				paillier_dec( plaintext ,pubkey,prvkey,ciphertext );
				char *ptext = malloc(1024);
				strcpy(ptext, "");
				mpz_get_str (ptext, 10,  plaintext->m);
				printf("ID: %s, Age: %s, Salary: %s\n", row[0], row[1], ptext);
			}
			printf("End of table\n");		
			mysql_free_result(res);
			mysql_more_results(conn);
			
		}
//SELECT SUM
		else if (	
		((wA1[0]=='s' || wA1[0]=='S') && (wA1[1]=='u' || wA1[1]=='U') 
		&& (wA1[2]=='m' || wA1[2]=='M')&& (wA1[3]=='\0'))	)
			{
				
			//Creates string for select sum query that will be concated with other strings to form query
			char *queryString = malloc(200); //might need to allocate more space
			strcpy(queryString, "");
			strncat(queryString, "SELECT ", 50);	
			char *sum_heString = malloc(200);
			strcpy(sum_heString, "SUM_HE(salary) "); 
			char *fromString = malloc(200);
			strcpy(fromString, "from Employees");
			char *ageString = malloc(200);
			strcpy(ageString, "age, ");
			
			char *countString = malloc(200);
			strcpy(countString, "count(id) ");
			
			//Creates substrings for Where, group by, having
			char *whereText = malloc(50);
			strcpy(whereText, "");
			char *groupText = malloc(50);
			strcpy(groupText, "");
			char *havingText = malloc(50);
			strcpy(havingText, "");
			//loops through wordArray, concatenating where, group by, and having substrings
			int h = 2; //start on third argument
			int subStringFlag = 0;
			int groupTrigger = 0;
			while(wordArray[h]!=NULL){	
			if(		//parses where substring; switches flag to 1 to add words to having substring
				(wordArray[h][0]=='W'||wordArray[h][0]=='w') &&
				(wordArray[h][1]=='H'||wordArray[h][1]=='h') &&
				(wordArray[h][2]=='E'||wordArray[h][2]=='e') &&
				(wordArray[h][3]=='R'||wordArray[h][3]=='r') &&
				(wordArray[h][4]=='E'||wordArray[h][4]=='e') &&
				(wordArray[h][5]=='\0'))
				{
					subStringFlag = 1;
				}
			if(		//finds group by statement; switches flag to 2 to add words to group by substring
				(wordArray[h][0]=='G'||wordArray[h][0]=='g') &&
				(wordArray[h][1]=='R'||wordArray[h][1]=='r') &&
				(wordArray[h][2]=='O'||wordArray[h][2]=='o') &&
				(wordArray[h][3]=='U'||wordArray[h][3]=='u') &&
				(wordArray[h][4]=='P'||wordArray[h][4]=='p') && (wordArray[h][5]=='\0'))
				{	
				if(i>3 &&
				(wordArray[h+1][0]=='B'||wordArray[h+1][0]=='b') &&
				(wordArray[h+1][1]=='Y'||wordArray[h+1][1]=='y')&& (wordArray[h+1][2]=='\0'))
					//printf("Group found\n");
					{
						subStringFlag = 2;
						groupTrigger=1;
					}
				}
			if(		//finds group by statement; switches flag to 3 to add words to having substring
				(wordArray[h][0]=='H'||wordArray[h][0]=='h') &&
				(wordArray[h][1]=='A'||wordArray[h][1]=='a') &&
				(wordArray[h][2]=='V'||wordArray[h][2]=='v') &&
				(wordArray[h][3]=='I'||wordArray[h][3]=='i') &&
				(wordArray[h][4]=='N'||wordArray[h][4]=='n') &&
				(wordArray[h][5]=='G'||wordArray[h][5]=='g') &&(wordArray[h][6]=='\0')
				)
				{
					//printf("Having found\n");
					subStringFlag = 3;
				}
			//concats where substring	
			//if flag==1, keep adding to where substring
			//printf("flag: %i, h: %i\n", subStringFlag, h);
			if(subStringFlag==1)
				{ 
				char *buffer = malloc(100);
				strcpy(buffer, wordArray[h]);
				strncat(whereText, spaceOnly, 50);	
				strncat(whereText, buffer, 50);	
				//printf("Inserting where: %s\n", whereText);
				free(buffer);
				}
			//concats group substring	
			if(subStringFlag==2)
				{ 
				char *buffer = malloc(100);
				strcpy(buffer, wordArray[h]);
				strncat(groupText, spaceOnly, 50);	
				strncat(groupText, buffer, 50);	
				//printf("Inserting group by: %s\n", groupText);
				free(buffer);
				}
			//concats having substring	
			if(subStringFlag==3)
				{ 
				char *buffer = malloc(100);
				strcpy(buffer, wordArray[h]);
				strncat(havingText, spaceOnly, 50);	
				strncat(havingText, buffer, 50);	
				//printf("Inserting where: %s\n", havingText);
				free(buffer);
				}
				
			//printf("%s\n",wordArray[h]);
			h++;
			}
			
			//Concatenating the substrings together to form query
			//if group by is detected, will include age into select argument (eg. select age, sum)
			if(groupTrigger==1){strncat(queryString, ageString, 50);}
			strncat(queryString, sum_heString, 50);
			strncat(queryString, fromString, 50);
			strncat(queryString, whereText, 50);
			strncat(queryString,  spaceOnly, 50);
			strncat(queryString, groupText, 50);
			strncat(queryString,  spaceOnly, 50);
			strncat(queryString, havingText, 50);
			strncat(queryString, semi, 50);
			printf("%s\n",queryString);
			
			//select sum query without group by statement ( results only have 1 column)
			if(groupTrigger==0){		
				if (mysql_query(conn, queryString))
					{
					fprintf(stderr, "%s\n", mysql_error(conn));
					}
				else{
		// printf("	calculating sum:\n");
					//printf("	Selected\n");
					res = mysql_use_result(conn);
					
					//Check for number of columns
					// int fields = mysql_field_count(conn);
					// printf("	number of fields: %i\n",fields);
					
					//Prints contents of next single row from mysql results
					 while((row = mysql_fetch_row(res))!=NULL)
						{
		//printf("row[0] is: %s\n",row[0]);
						//Decrypt and bring sum values
						paillier_plaintext_t *plaintext; 
						paillier_ciphertext_t *ciphertext;
						plaintext= (paillier_plaintext_t*)malloc(sizeof(paillier_plaintext_t));
						ciphertext= (paillier_ciphertext_t*)malloc(sizeof(paillier_ciphertext_t));
						mpz_init(plaintext->m);
						int val;	
						val=mpz_init_set_str(ciphertext->c, row[0], 10);
		//gmp_printf("cipher->c: %Zd\n", ciphertext->c);
						paillier_dec( plaintext ,pubkey,prvkey,ciphertext );
		//gmp_printf("plaintext translated: %Zd\n", plaintext->m);
						char *ptext = malloc(1024);
						strcpy(ptext, "");
						mpz_get_str (ptext, 10,  plaintext->m);
						
						//Checks if returned table is just empty
						char *row0 = malloc(1024);
						strcpy(row0, row[0]);
						if(row0[0]=='\0'){printf("Sum: NULL\n");}
						else
							{
								printf("Sum: %s \n", ptext);
							}
						}
					mysql_free_result(res);
					mysql_more_results(conn);
				}
			}
			//select sum query with group by (results should have 2 columns)
			if(groupTrigger==1){		
				if (mysql_query(conn, queryString))
					{
					fprintf(stderr, "%s\n", mysql_error(conn));
					}
				else{
			//printf("	calculating sum:\n");
					res = mysql_use_result(conn);
					
					//Check for number of columns (should be 2)
			// int fields = mysql_field_count(conn);
			// printf("	number of fields: %i\n",fields);
					
					//Prints contents of single row. If first row is empty, sum is empty
					 if((row = mysql_fetch_row(res))==NULL)
						{printf("Sum: NULL\n");}
					 else
					 {
						//loop through each grouping and print out sums
						do {
							//
							paillier_plaintext_t *plaintext; 
							paillier_ciphertext_t *ciphertext;
							plaintext= (paillier_plaintext_t*)malloc(sizeof(paillier_plaintext_t));
							ciphertext= (paillier_ciphertext_t*)malloc(sizeof(paillier_ciphertext_t));
							mpz_init(plaintext->m);

							int val;	
							val=mpz_init_set_str(ciphertext->c, row[1], 10);
							if(ciphertext->c==NULL){printf("empty table \n");}
			//gmp_printf("cipher->c: %Zd\n", ciphertext->c);

			
							paillier_dec( plaintext ,pubkey,prvkey,ciphertext );
			//gmp_printf("plaintext translated: %Zd\n", plaintext->m);
							char *ptext = malloc(1024);
							strcpy(ptext, "");
							mpz_get_str (ptext, 10,  plaintext->m);
							
							//check if table returns null sum (shouldn't ever, but might as well check)
							char *row0 = malloc(1024);
							strcpy(row0, row[1]);
							if(row0[0]=='\0'){printf("Sum: NULL\n");}
							else
							printf("Age: %s, Sum: %s \n", row[0],ptext);
						
							} while((row = mysql_fetch_row(res))!=NULL);
					 }
					mysql_free_result(res);
					mysql_more_results(conn);
					
				}
				
			}
			
		}
		
//SELECT AVG: 
		//Almost identical to sum, for each age grouping, does query of
		//select count(id) from Employees where age = (grouping)
		else if ((wA1[0]=='a' || wA1[0]=='A') && (wA1[1]=='v' || wA1[1]=='V') 
		&& (wA1[2]=='g' || wA1[2]=='G') && (wA1[3]=='\0'))
		
			{
			//Same substrings as in SUM
			char *queryString = malloc(200); 
			strcpy(queryString, "");
			strncat(queryString, "SELECT ", 50);	
			
			char *queryCountString = malloc(200); 
			strcpy(queryCountString, "");
			strncat(queryCountString, "SELECT ", 50);	
			
			char *sum_heString = malloc(200);
			strcpy(sum_heString, "SUM_HE(salary) "); 
			char *fromString = malloc(200);
			strcpy(fromString, "from Employees");
			char *ageString = malloc(200);
			strcpy(ageString, "age, ");
			
			//New substring, used to find count of rows in of age groupings
			char *countString = malloc(200);
			strcpy(countString, "count(id) ");
			
			char *whereText = malloc(50);
			strcpy(whereText, "");
			char *groupText = malloc(50);
			strcpy(groupText, "");
			char *havingText = malloc(50);
			strcpy(havingText, "");
			//loops through wordArray, concatenating where, group by, and having substrings
			int h = 2; //start on third argument
			int subStringFlag = 0;
			int groupTrigger = 0;
			while(wordArray[h]!=NULL){	
			if(		//parses where substring
				(wordArray[h][0]=='W'||wordArray[h][0]=='w') &&
				(wordArray[h][1]=='H'||wordArray[h][1]=='h') &&
				(wordArray[h][2]=='E'||wordArray[h][2]=='e') &&
				(wordArray[h][3]=='R'||wordArray[h][3]=='r') &&
				(wordArray[h][4]=='E'||wordArray[h][4]=='e') &&
				(wordArray[h][5]=='\0'))
				{
				//printf("Where found\n");
				subStringFlag = 1;
				}
			if(		//parses group by substring
				(wordArray[h][0]=='G'||wordArray[h][0]=='g') &&
				(wordArray[h][1]=='R'||wordArray[h][1]=='r') &&
				(wordArray[h][2]=='O'||wordArray[h][2]=='o') &&
				(wordArray[h][3]=='U'||wordArray[h][3]=='u') &&
				(wordArray[h][4]=='P'||wordArray[h][4]=='p') && (wordArray[h][5]=='\0'))
				{	
				if(i>3 &&
				(wordArray[h+1][0]=='B'||wordArray[h+1][0]=='b') &&
				(wordArray[h+1][1]=='Y'||wordArray[h+1][1]=='y')&& (wordArray[h+1][2]=='\0'))
					//printf("Group found\n");
					subStringFlag = 2;
					groupTrigger=1;
				}
			if(		//parses having substring
				(wordArray[h][0]=='H'||wordArray[h][0]=='h') &&
				(wordArray[h][1]=='A'||wordArray[h][1]=='a') &&
				(wordArray[h][2]=='V'||wordArray[h][2]=='v') &&
				(wordArray[h][3]=='I'||wordArray[h][3]=='i') &&
				(wordArray[h][4]=='N'||wordArray[h][4]=='n') &&
				(wordArray[h][5]=='G'||wordArray[h][5]=='g') &&(wordArray[h][6]=='\0')
				)
				{
					//printf("Having found\n");
					subStringFlag = 3;
				}
			//concats where substring	
			//printf("flag: %i, h: %i\n", subStringFlag, h);
			if(subStringFlag==1)
				{ 
				char *buffer = malloc(100);
				strcpy(buffer, wordArray[h]);
				strncat(whereText, spaceOnly, 50);	
				strncat(whereText, buffer, 50);	
				//printf("Inserting where: %s\n", whereText);
				free(buffer);
				}
			//concats group substring	
			if(subStringFlag==2)
				{ 
				char *buffer = malloc(100);
				strcpy(buffer, wordArray[h]);
				strncat(groupText, spaceOnly, 50);	
				strncat(groupText, buffer, 50);	
				//printf("Inserting group by: %s\n", groupText);
				free(buffer);
				}
			//concats having substring	
			if(subStringFlag==3)
				{ 
				char *buffer = malloc(100);
				strcpy(buffer, wordArray[h]);
				strncat(havingText, spaceOnly, 50);	
				strncat(havingText, buffer, 50);	
				//printf("Inserting where: %s\n", havingText);
				free(buffer);
				}
				
				
			//printf("%s\n",wordArray[h]);
			h++;
			}
			//concat query for sum
			if(groupTrigger==1){strncat(queryString, ageString, 50);}
			strncat(queryString, sum_heString, 50);
			strncat(queryString, fromString, 50);
			strncat(queryString, whereText, 50);
			strncat(queryString,  spaceOnly, 50);
			strncat(queryString, groupText, 50);
			strncat(queryString,  spaceOnly, 50);
			strncat(queryString, havingText, 50);
			strncat(queryString, semi, 50);
			//printf("%s\n",queryString);
			
			//concat query for row count //for select avg
			if(groupTrigger==1){strncat(queryCountString, ageString, 50);}
			strncat(queryCountString, countString, 50);
			strncat(queryCountString, fromString, 50);
			strncat(queryCountString, whereText, 50);
			strncat(queryCountString,  spaceOnly, 50);
			strncat(queryCountString, groupText, 50);
			strncat(queryCountString,  spaceOnly, 50);
			strncat(queryCountString, havingText, 50);
			strncat(queryCountString, semi, 50);
			//printf("%s\n",queryCountString);
			
			if(groupTrigger==0){		//select sum without group by (only 1 column)
				if (mysql_query(conn, queryString))
					{
					fprintf(stderr, "%s\n", mysql_error(conn));
					//exit(1);
					//break;
					}
				else{
		//printf("	calculating sum:\n");
					
					MYSQL_RES *storeRes;
					storeRes = mysql_store_result(conn);
					//res = mysql_use_result(conn);
					//MYSQL *tempConn = mysql_init(NULL);
					MYSQL_ROW tempRow = malloc(1024);

			//Check for number of columns (should be 1)
			// int fields = mysql_field_count(conn);
			// printf("	number of fields: %i\n",fields);
					
					
					//Prints contents of single row
					 while((row = mysql_fetch_row(storeRes))!=NULL)
						{
						//Mostly similar to Sum's
						paillier_plaintext_t *plaintext; 
						paillier_ciphertext_t *ciphertext;
						plaintext= (paillier_plaintext_t*)malloc(sizeof(paillier_plaintext_t));
						ciphertext= (paillier_ciphertext_t*)malloc(sizeof(paillier_ciphertext_t));
						mpz_init(plaintext->m);
						//if(ciphertext->c==0){printf("empty table \n");}
						int val;	
						val=mpz_init_set_str(ciphertext->c, row[0], 10);
		//gmp_printf("cipher->c: %Zd\n", ciphertext->c);

		
						paillier_dec( plaintext ,pubkey,prvkey,ciphertext );
		//gmp_printf("plaintext translated: %Zd\n", plaintext->m);
						char *ptext = malloc(1024);
						strcpy(ptext, "");
						mpz_get_str (ptext, 10,  plaintext->m);
						//Checks if returned table is just empty
						char *row0 = malloc(1024);
						strcpy(row0, row[0]);
						if(row0[0]=='\0'){printf("Average: NULL\n");}
						
						//Calculates average
						//Queries for the count of rows
						else
							{	
							//query: select count(id) from Employees
							printf("%s\n",queryCountString);
							if (mysql_query(conn, queryCountString))
								{
								fprintf(stderr, "%s\n", mysql_error(conn));
								}
							res=mysql_use_result(conn);	
							if((row = mysql_fetch_row(res))!=NULL)
								{
									//Calculates average from sum and newest query
									//printf("Count: %s ", row[0]);
									//printf("Sum: %s \n", ptext);
									int sum, count;
									sum = atoi(ptext);
									count = atoi(row[0]);
									double sumD, countD, avg;
									sumD = (double)sum;
									countD = (double)count;
									avg = sumD/countD;
									printf("Average : %f\n", avg);
									mysql_free_result(res);
									mysql_more_results(conn);
								}
							}
							
						}
					mysql_free_result(storeRes);
					mysql_more_results(conn);
				}
			}
			
			if(groupTrigger==1){		//select sum with group by substring (2 columns)
				if (mysql_query(conn, queryString))
					{
					fprintf(stderr, "%s\n", mysql_error(conn));
					}
				else{
		//printf("	calculating sum:\n");
					//printf("	Selected\n");
					MYSQL_RES *storeRes;
					storeRes = mysql_store_result(conn);
					MYSQL_ROW tempRow = malloc(1024);
					
		// //Check for number of columns (should be 2)
		// int fields = mysql_field_count(conn);
		// printf("	number of fields: %i\n",fields);
					
					//check contents first single row. If empty, return null sum
					 if((row = mysql_fetch_row(storeRes))==NULL)
						{printf("Average: NULL\n");}
					 else
					 {
						//loop through every age grouping
						do {
							//printf("enters do\n");
							paillier_plaintext_t *plaintext; 
							paillier_ciphertext_t *ciphertext;
							plaintext= (paillier_plaintext_t*)malloc(sizeof(paillier_plaintext_t));
							ciphertext= (paillier_ciphertext_t*)malloc(sizeof(paillier_ciphertext_t));
							mpz_init(plaintext->m);

							int val;	
							val=mpz_init_set_str(ciphertext->c, row[1], 10);
							if(ciphertext->c==NULL){printf("empty table \n");}
			//gmp_printf("cipher->c: %Zd\n", ciphertext->c);

			
							paillier_dec( plaintext ,pubkey,prvkey,ciphertext );
			//gmp_printf("plaintext translated: %Zd\n", plaintext->m);
							char *ptext = malloc(1024);
							strcpy(ptext, "");
							mpz_get_str (ptext, 10,  plaintext->m);
							
							//check if table returns null sum
							char *row0 = malloc(1024);
							strcpy(row0, row[1]);
							if(row0[0]=='\0'){printf("Average: NULL\n");}
							else
								{
									//Creates query for counting rows in each grouping
									//Query in format "Select count(id) from Employees where age = x"
									char *queryGroupRowCountString = malloc(200); //might need to allocate more space
									strcpy(queryGroupRowCountString, "");
									strncat(queryGroupRowCountString, "SELECT ", 50);	
									
									//concat query for group row count //for select avg group by age
									strncat(queryGroupRowCountString, countString, 50);
									strncat(queryGroupRowCountString, fromString, 50);
									strncat(queryGroupRowCountString, " where age = ", 50);
									strncat(queryGroupRowCountString, row[0], 50);
									strncat(queryGroupRowCountString, semi, 50);
									
					//printf("%s\n",queryGroupRowCountString);
									if (mysql_query(conn, queryGroupRowCountString))
										{
										fprintf(stderr, "%s\n", mysql_error(conn));
										}
									res=mysql_use_result(conn);	
									if((tempRow = mysql_fetch_row(res))!=NULL)
										{
											//Calculates average
								// printf("Count: %s ", tempRow[0]);
								// printf("Sum: %s \n", ptext);
											int sum, count;
											sum = atoi(ptext);
											count = atoi(tempRow[0]);
											double sumD, countD, avg;
											sumD = (double)sum;
											countD = (double)count;
											avg = sumD/countD;
											printf("Age: %s, Avg : %f\n", row[0], avg);
										}
										mysql_free_result(res);
										mysql_more_results(conn);
									}

								} while((row = mysql_fetch_row(storeRes))!=NULL);
					
						}
							mysql_free_result(storeRes);
							mysql_more_results(conn);
					 }
				}//
				
			}
			
		
//SELECT SINGLE QUERY
		//checks if there are only 2 arguments and not select all, eg Insert 25
		else if(i==2&&wA1[0]!='*') //if there are 2 arguments
			{
			//printf("	Select Single Query\n");
			//begins building string for query
			char *queryString = malloc(200);
			strcpy(queryString, ""); 
			//fills queryString with "select * from..."
			strncat(queryString, selectQuery, 50);	
			//concats argument 2 with queryString eg select * from 10
			strncat(queryString, wA1, 50);	
			strncat(queryString, selectQueryEnd, 50);
			
			//query in form: select * from Employees where id=x
			printf("%s\n",queryString);
			if (mysql_query(conn, queryString)) {
				fprintf(stderr, "%s\n", mysql_error(conn));
				}
			
			else
				{
				res = mysql_use_result(conn);
				
				//Check for number of columns (should be 3)
				//int fields = mysql_field_count(conn);
				//printf("	number of fields: %i\n",fields);
				
				//Prints contents of single row
				 if((row = mysql_fetch_row(res))==NULL)
				 {printf("No such employee exists in Employees\n");}
				else
					{
					//Decrypts Salary
					paillier_plaintext_t *plaintext; 
					paillier_ciphertext_t *ciphertext;
					plaintext= (paillier_plaintext_t*)malloc(sizeof(paillier_plaintext_t));
					ciphertext= (paillier_ciphertext_t*)malloc(sizeof(paillier_ciphertext_t));
					mpz_init(plaintext->m);

					int val;	
					val=mpz_init_set_str(ciphertext->c, row[2], 10);
					paillier_dec( plaintext ,pubkey,prvkey,ciphertext );
					char *ptext = malloc(1024);
					strcpy(ptext, "");
					mpz_get_str (ptext, 10,  plaintext->m);

					printf("ID: %s, Age: %s, Salary: %s\n", row[0], row[1], ptext);
					}
					mysql_free_result(res);
					mysql_more_results(conn);
				}
			free(queryString);
			queryString=NULL;
			}
		else {printf("Not a valid select query\n");}
		}
		
//INSERT
	else if((wA0[0]=='i' || wA0[0]=='I') && (wA0[1]=='n' || wA0[1]=='N')
	&& (wA0[2]=='s' || wA0[2]=='S') && (wA0[3]=='e' || wA0[3]=='E') 
	&& (wA0[4]=='r' || wA0[4]=='R') && (wA0[5]=='t' || wA0[5]=='T')
	&& (wA0[6]=='\0'))
		{
		//Create substring for 
	
		//Create string for insert query. Will be concated later on to form proper queries
		char *insertQuery = malloc(100);
		strcpy(insertQuery, "INSERT INTO Employees (id, age, salary) values (");
		char *insertQueryEnd = malloc(10);
		strcpy(insertQueryEnd, ");");
		
		char *queryString = malloc(1024); 
		strcpy(queryString, "");
		strncat(queryString, insertQuery, 50);
		
		//stores arguments into tokens for arg 3 and 4
		if(i>2){strcpy(wA2, wordArray[2]);}
		//salary
		if(i>3){
			strcpy(wA3, wordArray[3]);
			//Encrypt salary value
			paillier_plaintext_t *plaintext; 
			paillier_ciphertext_t *ciphertext;
			plaintext= (paillier_plaintext_t*)malloc(sizeof(paillier_plaintext_t));
			ciphertext= (paillier_ciphertext_t*)malloc(sizeof(paillier_ciphertext_t));
			//mpz_init(pubkey->n_squared);
			mpz_init(ciphertext->c);
	
			int val;	
			val=mpz_init_set_str(plaintext->m, wA3, 10);
//gmp_printf("Plaintext->m: %Zd\n", plaintext->m);
	
			paillier_enc( ciphertext, pubkey, plaintext, paillier_get_rand_devurandom );
//gmp_printf("cipher text %Zd\n", ciphertext->c);
			char *ctext = malloc(1024);
			strcpy(ctext, "");
			mpz_get_str (ctext, 10, ciphertext->c);
//printf("CipherText: %s\n",ctext);
			strcpy(wA3, ctext);
			}
		
		//Appending arguments into query
		if(i>1)	
			{
			strncat(queryString, wA1, 20);
			}
		if(i>2)
			{
			strncat(queryString, space, 10);
			strncat(queryString, wA2, 20);	
			}
		if(i>3)
			{
			strncat(queryString, space, 10);
			strncat(queryString, wA3, 200);
			}
		
		if(i>4)//purposely append 4th argument if there is one to have mysql throw an error
			{
			strcpy(wA4, wordArray[4]);
			strncat(queryString, space, 10);
			strncat(queryString, wA4, 200);
			}
		strncat(queryString, insertQueryEnd, 10);
		printf("%s\n",queryString);

		//insert query string
		if (mysql_query(conn, queryString)) {
			fprintf(stderr, "%s\n", mysql_error(conn));
			}
		else {
			//printf("	Inserted into table\n");
			}
		}
	else {printf("Not a valid Query\n");}
	//return 0;	
	}
}