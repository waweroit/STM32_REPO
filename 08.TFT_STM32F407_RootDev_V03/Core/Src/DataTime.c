/*
 * DataTime.c
 *
 *  Created on: Jan 12, 2025
 *      Author: wawer
 */

#include "DataTime.h"


volatile ActualDate DateNow={0};

bool ProcessNTPResponse(const uint8_t *response)
{
    // Wyciągnij Transmit Timestamp (bajty 40-43)
    uint32_t ntpSeconds = (response[40] << 24) | (response[41] << 16) | (response[42] << 8) | response[43];

    // Konwersja czasu NTP do czasu UNIX
    uint32_t unixTime = ntpSeconds - 2208988800U;

    // Zmienna do przechowywania daty/godziny
    int year, month, day, hour, minute, second;

    // Algorytm konwersji czasu UNIX na datę/godzinę (uwzględnia lata przestępne)
    // Liczenie lat
    year = 1970;
    while (1)
    {
        uint32_t secondsInYear = IsLeapYear(year) ? 31622400 : 31536000; // Rok przestępny ma 31622400 sekund
        if (unixTime >= secondsInYear)
        {
            unixTime -= secondsInYear;
            year++;
        }
        else
        {
            break;
        }
    }

    // Liczba dni w miesiącach
    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    month = 0;
    while (1)
    {
        int days = daysInMonth[month];

        // Sprawdzenie lutego dla roku przestępnego
        if (month == 1 && IsLeapYear(year))
        {
            days = 29; // Luty w roku przestępnym
        }

        if (unixTime >= days * 86400)  // 86400 sekund w dniu
        {
            unixTime -= days * 86400;
            month++;
        }
        else
        {
            break;
        }
    }
    month++;  // Przechodzimy na numer miesiąca (1-12)

    // Obliczenie dnia, godzin, minut i sekund
    day = unixTime / 86400 + 1; // Dodajemy 1, aby uzyskać rzeczywisty dzień miesiąca
    unixTime %= 86400;

    hour = unixTime / 3600;
    unixTime %= 3600;

    minute = unixTime / 60;
    second = unixTime % 60;
    if (year ==2036 && month == 2 && day == 7 && hour == 6 && minute == 28 && second == 16)
	{
	// improper data
    	if(debugOn)
    		Debug("Inproper Data\r\n");
    	return false;
	}
    // Wyświetlanie daty/godziny

    // Wyznacz dzień tygodnia
    DateNow.year = year;
    DateNow.month = month;
    DateNow.day = day;

    if(DateNow.isWinterTime == true)
        DateNow.hour = hour+1;
    else
        DateNow.hour = hour;

    DateNow.minute = minute;
    DateNow.second = second;

	procesNTPTime = true;
	if(TimerInit == false)
	  if (HAL_TIM_Base_Start_IT(TIMERSEC) == HAL_OK)
	  {
		  TimerInit = true;
	  }

    const char *dayOfWeek = GetDayOfWeek(year, month, day);
    strcpy((void*)DateNow.DayName, dayOfWeek);

    //char debugData[200];

    //snprintf (debugData, sizeof(debugData), "NTP   Date and Time: %04d-%02d-%02d %02d:%02d:%02d (%s)\n Dobrych pobran: %d, ogolnie polaczen do NTP: %d.\r\n", DateNow.year, DateNow.month, DateNow.day, DateNow.hour, DateNow.minute, DateNow.second, dayOfWeek, countGood, connections);
    if(debugOn)
    {
		snprintf (debugData, sizeof(debugData), "NTP   Date and Time: %04d-%02d-%02d %02d:%02d:%02d (%s)\r\n", DateNow.year, DateNow.month, DateNow.day, DateNow.hour, DateNow.minute, DateNow.second, dayOfWeek);
		Debug(debugData);
    }


//    countGood++;
    return true;
}


void IncrementLocalTime(ActualDate *date)
{
    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Inkrementuj sekundy
    date->second++;
    if (date->second >= 60) {
        date->second = 0;
        date->minute++;
    }

    // Inkrementuj minuty
    if (date->minute >= 60) {
        date->minute = 0;
        date->hour++;
    }

    // Inkrementuj godziny
    if (date->hour >= 24) {
        date->hour = 0;
        date->day++;
    }

    // Liczba dni w bieżącym miesiącu
    int maxDays = daysInMonth[date->month - 1];

    // Sprawdzenie lutego w roku przestępnym
    if (date->month == 2 && IsLeapYear(date->year)) {
        maxDays = 29;
    }

    // Przejście do następnego miesiąca
    if (date->day > maxDays) {
        date->day = 1;
        date->month++;
    }

    // Przejście do następnego roku
    if (date->month > 12) {
        date->month = 1;
        date->year++;
    }

    // Aktualizacja nazwy dnia tygodnia
    const char *dayOfWeek = GetDayOfWeek(date->year, date->month, date->day);
    strcpy(date->DayName, dayOfWeek);
}

const char *GetDayOfWeek(int year, int month, int day)
{
    // Tablica nazw dni tygodnia
    static const char *daysOfWeek[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday","Sunday"};

    // Poprawka dla miesięcy: styczeń i luty liczone jako miesiące 13 i 14 poprzedniego roku
    if (month < 3)
    {
        month += 12;
        year--;
    }

    // Algorytm Zeller'a do obliczenia dnia tygodnia
    int K = year % 100;      // Rok stulecia
    int J = year / 100;      // Numer stulecia
    int dayOfWeek = (day + (13 * (month + 1)) / 5 + K + (K / 4) + (J / 4) - (2 * J)) % 7;

    // Zeller zwraca 0 = sobota, więc mapujemy na 0 = niedziela
    dayOfWeek = (dayOfWeek + 5) % 7;

    return daysOfWeek[dayOfWeek];
}

// Funkcja do sprawdzenia, czy rok jest przestępny
int IsLeapYear(int year)
{
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}
