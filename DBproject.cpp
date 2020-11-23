#include <stdio.h>
#include <iostream>
#include <string.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

using namespace std;

const char* host = "localhost";
const char* user = "root";
const char* pw = "Ghkdkrtks1!";
const char* db = "dbproject2";

int main(void) {

	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	else
	{
		const char* query;
		char readQ[500];
		int state = 0;
		printf("Database Connection Succeed\n");
		printf("Creating Tables.....please wait\n");
		// Execute CRUD
		FILE* fr = fopen("20161608.txt", "r");
		while (fgets(readQ, 500, fr) != NULL) {
			for (int i = 0; i < 500; i++) {
				if (readQ[i] == '\n') {
					readQ[i] = '\0';
					break;
				}
			}
			//printf("%s\n", readQ);
			//cout << readQ << endl;
			mysql_query(connection, readQ);

			//cout << endl;
		}
		printf("Creating Tables complete\n");

		while(1){
			
			printf("\n------ SELECT QUERY TYPES -------\n\n");
			printf("\t1. TYPE I\n");
			printf("\t2. TYPE II\n");
			printf("\t3. TYPE III\n");
			printf("\t4. TYPE IV\n");
			printf("\t5. TYPE V\n");
			printf("\t0. QUIT\n");
			int menu;
			printf("Input query type : ");
			//cout << "Input query type : ";
			cin >> menu;
			if (menu == 0) {
				printf("Bye!\n");
				//cout << "Bye!\n";
				break;
			}
			else if (menu == 1) {
				printf("You choose <TYPE I>\n");
				//cout << "You choose <TYPE I>" << endl;
				while (1) {
					printf("\n------- Subtypes in TYPE I -------\n");
					printf("Assume truct is destroyed in crash\n");
					printf("Assume destroyed truck's ship_code is 50000014 in this Program\n\n");
					printf("\t1. TYPE I-1\n");
					printf("\t2. TYPE I-2\n");
					printf("\t3. TYPE I-3\n");
					printf("\t0. OUT of TYPE-I\n\n");
					
					printf("Input subtype : ");
					cin >> menu;
					if (menu == 0) break;
					else if (menu == 1) {
						printf("You choose <TYPE I-1>\n");
						printf("**Find all customers who had a package on the truck at the time of the crash.**\n");
						query = "select name from account where account_number in (\
									select ac_num from record where b_code in(\
										select b_code from purchase where p_code in(\
											select p_code from transport where sh_code = 50000014)\
										)\
								)";
						state = mysql_query(connection, query);
						if (state == 0) {
							printf("Answer for <TYPE I - 1> : \n");
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
								printf("\t%s\n", sql_row[0]);
							}
							mysql_free_result(sql_result);
							printf("\n");
						}

					}
					else if (menu == 2) {
						printf("You choose <TYPE I-2>\n");
						printf("**Find all recipients who had a package on that truck at the time of the crash.**\n");
						query = "select rec_name from package where package_code in (select p_code from transport where sh_code = 50000014)";
						state = mysql_query(connection, query);
						printf("Answer for <TYPE II-2>\n");
						if (state == 0) {
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
								printf("\t%s\n", sql_row[0]);
							}
							mysql_free_result(sql_result);
						}
						printf("\n");

					}
					else if (menu == 3) {
						printf("You choose <TYPE I-3>\n");
						printf("**Find the last successful delivery by that truck prior to the crash.**\n");
						printf("Assume that today is 2020-05-20\n");
						printf("\n");
						query = "select * from package where (\
							timeline < deadline and timeline <= 20200520 and\
							package_code in(select p_code from transport where sh_code = 50000014));";
						state = mysql_query(connection, query);
						printf("Answer for <TYPE I-3>\n");
						printf("\n");
						if (state == 0) {
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
								printf("\t[%s] already had received package in [%s]\n", sql_row[5], sql_row[6]);
							}
							mysql_free_result(sql_result);
						}
						printf("\n");

					}
					else {
						printf("Your input is wrong! Input Again(subtype of TYPE I)\n");
						printf("Quit for 0\n");
					}
				}
				
			} // menu(1)
			else if (menu == 2) {
				char inyear[10];
				printf("You choose <TYPE II>\n");
				printf("**Find the customer who has shipped the most packages in the past certain year.**\n");
				std::cout << "Which year? : ";
				cin >> inyear;

				char tempquery[500] = "with T as (select bill_code from bill where year = ";
				strcat_s(tempquery, inyear);
				strcat_s(tempquery, ")");
				char tempstr[500] = "select name from account where account_number in(\
						select ac_num from record group by ac_num\
						having count(*) = (select max(mycount)\
							from(select ac_num, count(*) as mycount from record, T\
				where record.b_code = T.bill_code\
				group by ac_num) as result1))";
				
				strcat_s(tempquery, tempstr);
				
				state = mysql_query(connection, tempquery);
				int cnt = 0;
				if (state == 0) {
					std::cout << "Answer for <TYPE II> : " << endl;
					std::cout << "Customer Name : " << endl;
					sql_result = mysql_store_result(connection);
					
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
						printf("\t%s\n", sql_row[0]);
						cnt++;
					}
					mysql_free_result(sql_result);
				}
				if (cnt == 0) std::cout << "There is no such satisfied query" << endl;
				

			} // menu(2)
			else if (menu == 3) {
				char inyear[10];
				std::cout << "You choose <TYPE III>" << endl;
				std::cout << "**Find the customer who has spent the most money on shipping in the past certain year.**" << endl;
				std::cout << "Which year? : ";
				cin >> inyear;

				char tempQ[500] = "with R as (select bill_code, pay_amount from bill where year = ";
				strcat_s(tempQ, inyear);
				strcat_s(tempQ, ")");

				char tempstr[500] = "select ac_num, sum(pay_amount) from record, R where b_code = R.bill_code group by ac_num";
				strcat_s(tempQ, tempstr);

				
				int cnt = 0;
				char max_ac_num[30];
				float max_pay_amount = 0.0;
				int cnt3 = 0;

				state = mysql_query(connection, tempQ);
				if (state == 0) {
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
						cnt3++;
						if (max_pay_amount <= strtof(sql_row[1], NULL)) {
							max_pay_amount = strtof(sql_row[1], NULL);
							strcpy_s(max_ac_num, sql_row[0]);
						}
					}
					mysql_free_result(sql_result);
					
				}
				if (cnt3 == 0) std::cout << "There is no such satisfied query" << endl;
				else {
					//printf("%s, %.5f\n", max_ac_num, max_pay_amount);
					char Q[500] = "select name from account where account_number = ";
					strcat_s(Q, max_ac_num);
					state = mysql_query(connection, Q);
					if (state == 0) {
						std::cout << "Answer for <TYPE IV> : " << endl;
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
							std::cout << "\tCustomer name : " << sql_row[0] << endl;
						}

					}
				}

			}
			else if (menu == 4) {
				std::cout << "You choose <TYPE IV>" << endl;
				std::cout << "**Find those packages that were not delivered within the promised time.**" << endl;
				
				query = "select rec_name, package_code from package where timeline > deadline";
				state = mysql_query(connection, query);
				if (state == 0){
					std::cout << "Answer for <TYPE IV> : " << endl;
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL){
						printf("\tpackage code [%s] was not delivered in time to [%s]\n", sql_row[1], sql_row[0]);
					}
					mysql_free_result(sql_result);
				}
				else {
					std::cout << "There is no such satisfied query" << endl;
				}

				mysql_close(connection);

			}
			else if (menu == 5) {
				state = 0;
				char buffer;
				char inname[30];
				char inac_num[30]; // input ac_num

				for (int i = 0; i < 30; i++) {
					inname[i] = '\0';
					inac_num[i] = '\0';
				}

				std::cout << "You choose <TYPE V>" << endl;
				std::cout << "**Generate the bill for each customer for the past certain month.**" << endl;
				std::cout << "Customer name : ";
				scanf_s("%c", &buffer);
				fgets(inname, 30, stdin);
				
				// remove '\n' in string inname[]
				for (int i = 0; i < 30; i++) {
					if (inname[i] == '\n') {
						inname[i] = '\0';
						break;
					}
				}

				// find account_number of <inname>
				// make query : select account_number from account where name = [inname]
				char tempquery[500] = "select account_number from account where name = '";
				strcat_s(tempquery, inname);
				strcat_s(tempquery, "'");
				//cout << tempquery << endl;

				// run the query
				state = mysql_query(connection, tempquery);
				if (state == 0) {
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
						strcpy_s(inac_num, sql_row[0]);
					}
					mysql_free_result(sql_result);
				}
				//mysql_close(connection);
				if (inac_num[0] == '\0') std::cout << "There is no such satisfied query" << endl;
				else {
					float total_amount = 0.0; // total paid amount
					char filename[100] = { 0, };
					strcpy_s(filename, "Bill_Text_File_");
					strcat_s(filename, inname);
					strcat_s(filename, ".txt");
					
					FILE* fw = fopen(filename, "w");
					fprintf(fw, "Purchase Record of %s\n", inname);
					fprintf(fw, "=================================\n");

					//printf("find account_number : %s -> %s\n", inname, inac_num);
					char innerQ[500] = "select b_code from record where ac_num = ";
					strcat_s(innerQ, inac_num);
					char outerQ[500] = "select pay_amount, pay_option, year, month, day from bill where bill_code in (";
					strcat_s(outerQ, innerQ);
					strcat_s(outerQ, ")");

					state = mysql_query(connection, outerQ);
					if (state == 0) {
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
							fprintf(fw, "pay_amount = %6s, pay_option = %5s, date : %s-%02s-%02s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
							total_amount += strtof(sql_row[0], NULL); // calculate total paid amount, change string into float
						}
						mysql_free_result(sql_result);
						fprintf(fw, "Total : %.4f\n", total_amount);
						fclose(fw);
						std::cout << "Generating Bill is completed" << endl;
					}
				}
			}
			else {
				printf("Your input is wrong!\n\n\n");
			}
		}
	}

	return 0;
}
