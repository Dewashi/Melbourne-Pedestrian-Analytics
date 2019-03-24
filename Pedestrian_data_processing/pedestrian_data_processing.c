/*Created by Deevesh Pillay Shanmuganathan*/
#include <stdio.h>
#include <math.h>

#define MAX_DAYS 10000                    /*Max amout of days per tsv file*/
#define NUM_HEADING_LINES 2               /*Number of newlines in heading*/
#define NUM_GROUPS 3                       /*Number of groups for stage 4*/
#define FEB_LEAP_YEAR_DAYS 29              /*Days in short February month*/
#define FEB_NON_LEAP_YEAR_DAYS 28           /*Days in long February month*/
#define LONGER_MONTH 31              /*Days in a long month*/
#define SHORTER_MONTH 30             /*Days in a short month*/
#define JANUARY_MONTH_NUM 1             /*Value for each corresponding month*/
#define FEBRUARY_MONTH_NUM 2
#define MARCH_MONTH_NUM 3
#define APRIL_MONTH_NUM 4
#define MAY_MONTH_NUM 5
#define JUNE_MONTH_NUM 6
#define JULY_MONTH_NUM 7
#define AUGUST_MONTH_NUM 8
#define SEPTEMBER_MONTH_NUM 9
#define OCTOBER_MONTH_NUM 10
#define NOVEMBER_MONTH_NUM 11
#define DECEMBER_MONTH_NUM 12
#define MONDAY_DAY_NUM 2                   /*Value for each corresponding day*/
#define TUESDAY_DAY_NUM 3
#define WEDNESDAY_DAY_NUM 4
#define THURSDAY_DAY_NUM 5
#define FRIDAY_DAY_NUM 6
#define SATURDAY_DAY_NUM 7
#define SUNDAY_DAY_NUM 1

int datafill(int date_year[], int date_month[], int date_day[], int day[],
int day_count[]);
int mygetchar();
void range_and_ratio_calc(int date_year[], int date_month[],
int date_day[], int num_records);
int days_calc(int year, int month, int day);
void Monthly_Count(int date_year[], int date_month[],
int day_count[], int num_records);
int calc_days_in_month(int month, int year);
void calc_trends(int date_year[], int date_month[], int day_date[], int day[],
int day_count[], int num_records);
void print_group_details(int record_num, int group_start, int group_limit,
 int date_year[], int date_month[], int day_date[], int group_total_records);
void print_records(int week_day, int day[], int group_record_start,
int group_record_end, int day_count[], int group_num);
void print_day_name(int week_day);
void print_bar_chart(double num_stars);

/*runs the show*/
int main(int argc, char** argv){
    int date_year[MAX_DAYS], date_month[MAX_DAYS], date_day[MAX_DAYS];
    int day[MAX_DAYS], day_count[MAX_DAYS], num_records;
    /*num records returned as it is usueful for later functions*/
    num_records = datafill(date_year, date_month, date_day, day, day_count);
    range_and_ratio_calc(date_year, date_month, date_day, num_records);
    Monthly_Count(date_year, date_month, day_count, num_records);
    calc_trends(date_year, date_month, date_day, day, day_count, num_records);
    return(0);
}

/*Based on code written by Alistair Moffat on
 https://people.eng.unimelb.edu.au/ammoffat/teaching/20005/ass1/
 Gets a character and stores it in a variable*/
int mygetchar() {
	int c;
	while ((c=getchar())=='\r') {
	}
	return c;
}

/*S1: Fills up relevant arrays with traffic data*/
int datafill(int date_year[], int date_month[], int date_day[], int day[],
int day_count[]){
    int num_records = 0, count_newline=0;
    char next_char;
    /*Scans and discards the heading of the input TSV file
    until the end of line 2 has been reached*/
    while(count_newline != NUM_HEADING_LINES){
        next_char= mygetchar();
        if (next_char == '\n'){
            count_newline += 1;
        }
        if (count_newline == NUM_HEADING_LINES){
            break;
        }
    }

    /*Stores scanned data from a input TSV file into arrays, reading them
    in a set format*/
    while(scanf("%d%d%d%d%d",&date_year[num_records], &date_month[num_records],
    &date_day[num_records], &day[num_records], &day_count[num_records]) == 5){
         num_records += 1;
    }
    /*print the required output for S1*/
    printf("S1: total data lines = %d days\n", num_records);
    printf("S1: first data line  = %02d/%02d/%04d, %d people counted\n",
    date_day[0], date_month[0],date_year[0], day_count[0]);
    printf("S1: last data line   = %02d/%02d/%04d, %d people counted\n\n",
    date_day[num_records-1], date_month[num_records-1], date_year[num_records-1]
    , day_count[num_records-1]);
    return(num_records);
}

/*S2: Calculates the range of the input data and the ratio*/
void range_and_ratio_calc(int date_year[],int date_month[],int date_day[],
int num_records){
    int start_day, final_day, range;
    double ratio;
    start_day = days_calc(date_year[0], date_month[0], date_day[0]);
    final_day = days_calc(date_year[num_records-1], date_month[num_records-1],
    date_day[num_records-1]);
    /*Calculates the range of data, depending on the calculated start and end
    dates of the input values*/
    range = final_day - start_day + 1;
    printf("S2: range spanned    = %d days\n", range);
    ratio = 100.0*num_records/range;
    printf("S2: coverage ratio   = %4.1f%%\n\n", ratio);
    return;
}

/*S2: Calculates the amount of days in a given date, as specified by the formula
derived from the website https://math.stackexchange.com/questions/683312/for
mula-to-calculate-difference-between-two-dates
Values are not hash defined as the numbers used are unque to the formula*/
int days_calc(int year,int month, int day){
    if (month <=2){
        month = month+12;
        year = year-1;
    }
    return floor((146097*year/400.0) + ((153*month + 8)/5) + day);
}

/*S3: Counts the averge amount of pedestrians (expressed in multiples of 1k) for
 a specific month and counts the amount of days which contribute to
 the average for that month*/
void Monthly_Count(int date_year[], int date_month[], int day_count[],
int num_records){
    int i, prev_year=date_year[0], prev_month=date_month[0],
    ped_tally=day_count[0],  day_tally=1, days_in_month;
    /*Itterates through the number of records*/
    for (i=1; i<num_records; i++){
        /*Checks to see if the previous supplied date is equal to the one
        being processed and if so increases the pedestrain count
         and increments the day tally by 1*/
        if (prev_year == date_year[i] && prev_month == date_month[i]){
            ped_tally += day_count[i];
            day_tally += 1;
            prev_year = date_year[i];
            prev_month = date_month[i];
            /*Checks if the record currently being procesed is the last one and
            if so prints the tally of days coverd and the average pedestrains*/
            if (i == num_records-1){
                days_in_month = calc_days_in_month(date_month[i], date_year[i]);
                printf("S3: %02d/%04d %2d/%2d days covered, average count =  "
                "%4.1fk\n", date_month[i], date_year[i],day_tally, days_in_month
                , ped_tally/1000.0/day_tally);
            }
        }
        /*If the previous year and month and month are not the same as the one
        being processed then the day and pedestrian tally will be printed
        for the previous month and year*/
        else{
            days_in_month = calc_days_in_month(date_month[i-1], date_year[i-1]);
            printf("S3: %02d/%04d %2d/%2d days covered, average count =  "
            "%4.1fk\n", date_month[i-1], date_year[i-1],day_tally,
            days_in_month, ped_tally/1000.0/day_tally);
            ped_tally = day_count[i];
            day_tally = 1;
            prev_year = date_year[i];
            prev_month = date_month[i];
            /*Checks if the record currently being procesed is the last one and
            if so prints the tally of days coverd and the average pedestrains.
            Statement repeated to account for branching conditions*/
            if (i == num_records-1){
                days_in_month = calc_days_in_month(date_month[i], date_year[i]);
                printf("S3: %02d/%04d %2d/%2d days covered, average count =  "
                "%4.1fk\n", date_month[i], date_year[i],day_tally, days_in_month
                ,day_count[i]/1000.0);
            }
        }
    }
    printf("\n");

    return;
}

/*S3: Calculates the days in a particular month based on facts*/
int calc_days_in_month(int month, int year){
    if (month == JANUARY_MONTH_NUM|| month == MARCH_MONTH_NUM ||
        month == MAY_MONTH_NUM || month == JULY_MONTH_NUM ||
        month == AUGUST_MONTH_NUM || month == OCTOBER_MONTH_NUM ||
        month == DECEMBER_MONTH_NUM){
            return LONGER_MONTH;
    }
    if (month == APRIL_MONTH_NUM || month == JUNE_MONTH_NUM ||
        month == SEPTEMBER_MONTH_NUM || month == NOVEMBER_MONTH_NUM){
        return SHORTER_MONTH;
    }
    /*February has 29 days on a leap year and 28 days otherwise*/
    if (month ==  FEBRUARY_MONTH_NUM){
        /*conditions for a leap year, numbers not hash defined as they are
        a part of a formula*/
        /*code segment for leap years based on code made by Alistair Moffat
        from this link https://people.eng.unimelb.edu.au/ammoffat/teaching/
        20005/datetomorrow.c*/
        if(year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)){
            return FEB_LEAP_YEAR_DAYS;
        }
        else{
            return FEB_NON_LEAP_YEAR_DAYS;
        }
    }
    return(0);
}

/*S4: Calculates bar graphs based on the trends of pedestrians counts when
all records are divided into a certain number of groups and are sorted by days*/
void calc_trends(int date_year[], int date_month[], int day_date[], int day[],
int day_count[], int num_records){
    int j, i=0, week_day, count_record=-1, group_limit[MAX_DAYS],
    group_start[MAX_DAYS], records_per_group,extra_records, group_total_records;
    /*S4: determines the default number of recods per group and how many
     leftover records there are after being split into groups*/
    records_per_group = num_records / NUM_GROUPS;
    extra_records = num_records % NUM_GROUPS;
    /*S4: Iterates through each group of records and determines if a group
    should have an additional record or not, based on how
    many extra records there are (extra records are assigned to groups
    by descending group numbers)*/
    for (j=0; j<NUM_GROUPS; j++){
        group_start[j] = count_record + 1;
        if (j >= (NUM_GROUPS - extra_records)){
            count_record += records_per_group + 1;
            group_limit[j] = count_record;
            group_total_records = records_per_group + 1;
        }
        else{
            count_record += records_per_group;
            group_limit[j] = count_record;
            group_total_records = records_per_group;
        }
        /*S4: prints the values associated with a group*/
        print_group_details(j ,group_start[j],
        group_limit[j], date_year, date_month, day_date, group_total_records);
    }
    printf("\n");
    /*S4: Sends each group as an input and prints out relevant details
    based on certain days*/
    for(i=1; i<=7; i++){
        week_day = i;
        for (j=0; j<NUM_GROUPS; j++){
            print_records(week_day, day, group_start[j], group_limit[j],
            day_count, j);
        }
        printf("\n");
    }
    return;
}

/*S4: prints the number of records in each data group and the span of the
data between dates*/
void print_group_details(int record_num,
int group_start,int group_limit, int date_year[],
int date_month[], int day_date[], int group_total_records){
    /*prints group details*/
    printf("S4: group %d data, %02d/%02d/%04d to %02d/%02d/%04d, %d data "
    "records\n",record_num,day_date[group_start], date_month[group_start],
     date_year[group_start], day_date[group_limit], date_month[group_limit],
    date_year[group_limit], group_total_records);
    return;
}

/*S4: prints the calculated pederstrian tally for each group on a acertain day*/
void print_records(int week_day, int day[],int group_record_start,
int group_record_end, int day_count[], int group_num){
    int i, ped_tally=0, day_tally=0;
    day_tally = 0;
    ped_tally = 0;
    for(i=group_record_start; i<=group_record_end; i++){
        /*checks to see if the day in the record being processed matches
        the required day and if so adds the record to the tally*/
        if (week_day == day[i]){
             ped_tally+=day_count[i];
             day_tally+=1;
        }
    }
    /*S4: prints the output of stage 4*/
    /*pedestrain count displayed as a multiple of 1k*/
    print_day_name(week_day);
    printf(", g%d = %4.1fk |",group_num, ped_tally/1000.0/day_tally);
    print_bar_chart(ped_tally/1000.0/day_tally);
    return;
}

/*S4: prints the name of a day according the the input value of the day of the
group being test, as a part of stage 4*/
void print_day_name(int week_day){
    if (week_day == SUNDAY_DAY_NUM){
        printf("S4: Sun");
    }
    else if (week_day == MONDAY_DAY_NUM){
        printf("S4: Mon");
    }
    else if (week_day == TUESDAY_DAY_NUM){
        printf("S4: Tue");
    }
    else if (week_day == WEDNESDAY_DAY_NUM){
        printf("S4: Wed");
    }
    else if (week_day == THURSDAY_DAY_NUM){
        printf("S4: Thu");
    }
    else if (week_day == FRIDAY_DAY_NUM){
        printf("S4: Fri");
    }
    else if (week_day == SATURDAY_DAY_NUM){
        printf("S4: Sat");
    }
    return;
}

/*S4: prints the number of stars in correlation to the average pedestran tally
per day divided by 1000*/
void print_bar_chart(double num_stars){
    int i;
    num_stars = round(num_stars);
    for(i=1; i<=num_stars; i++){
        printf("*");
    }
    printf("\n");
    return;
}
/*programming is fun*/
