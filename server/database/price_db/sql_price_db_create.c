#include <stdio.h>
#include <sqlite3.h>

int main(){
	sqlite3 *db;

	// Construct the SQL query using sprintf
	char insert_query[1000];  // Adjust the size accordingly

	int rc = sqlite3_open("parking_prices_per_city.db", &db);
	if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    // Handle the error, possibly close resources and exit the program
	}	
	
	//const char *create_table_query = "CREATE TABLE IF NOT EXISTS your_table (col1 INT, col2 TEXT, col3 REAL, col4 TEXT);";
	const char *create_table_query = "CREATE TABLE IF NOT EXISTS city_parking (CITY TEXT, PRICE REAL);";
	
	rc = sqlite3_exec(db, create_table_query, 0, 0, 0);
	if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    // Handle the error, possibly close resources and exit the program
	}
	
	//sprintf(insert_query, "INSERT INTO your_table (col1, col2, col3, col4) VALUES (%d, '%s', %f, '%s');",
    //    value1, value2, value3, value4);
    sprintf(insert_query, "INSERT INTO city_parking (CITY , PRICE ) VALUES ('%s', %f);",
        "Ashkelon", 0.006);
	rc = sqlite3_exec(db, insert_query, 0, 0, 0);
	if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    // Handle the error, possibly close resources and exit the program
	}
	
	sprintf(insert_query, "INSERT INTO city_parking (CITY , PRICE ) VALUES ('%s', %f);",
        "Jerusalem", 0.012);
	rc = sqlite3_exec(db, insert_query, 0, 0, 0);
	if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    // Handle the error, possibly close resources and exit the program
	}
	
	sprintf(insert_query, "INSERT INTO city_parking (CITY , PRICE ) VALUES ('%s', %f);",
        "Petah-Tikva", 0.008);
	rc = sqlite3_exec(db, insert_query, 0, 0, 0);
	if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    // Handle the error, possibly close resources and exit the program
	}
	
	sprintf(insert_query, "INSERT INTO city_parking (CITY , PRICE ) VALUES ('%s', %f);",
        "Herzliya", 0.010);
	rc = sqlite3_exec(db, insert_query, 0, 0, 0);
	if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    // Handle the error, possibly close resources and exit the program
	}
	
	sqlite3_close(db);
	return 0;
}
