#include "font.h"

int get_length(int num) {
  int length = 0;
  while (num != 0) {
    length++;
    num /= 10;
  }
  return length;
}

void divide_decimal(float num, int *before_decimal, int *after_decimal) {
  if (isnan(num)) {
    *before_decimal = 0;
    *after_decimal = 0;
    return;
  }
  *before_decimal = (int)num;
  *after_decimal = (int)(round((num - *before_decimal) * 100));
}

int **generate_temp(float temp) {
  int before_decimal;
  int after_decimal;
  divide_decimal(temp, &before_decimal, &after_decimal);
  // Allocate memory for the matrix
  int **matrix = (int **)malloc(8 * sizeof(int *));
  for (int i = 0; i < 8; i++) {
    matrix[i] = (int *)malloc(64 * sizeof(int));
  }

  int before_decimal_first_digit = 0;
  int before_decimal_second_digit = before_decimal;
  if (get_length(before_decimal) != 1) {
    before_decimal_second_digit = before_decimal % 10;
    before_decimal /= 10;
    before_decimal_first_digit = before_decimal % 10;
    before_decimal /= 10;
  }

  int after_decimal_first_digit = 0;
  int after_decimal_second_digit = after_decimal;
  if (get_length(after_decimal) != 1) {
    after_decimal_second_digit = after_decimal % 10;
    after_decimal /= 10;
    after_decimal_first_digit = after_decimal % 10;
    after_decimal /= 10;
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 64; j++) {
      matrix[i][j] = 0;
    }
  }
  int start_position[2] = { 1, 15 };

  char text[] = "TEMP";
  int pos = 0;
  while (text[pos] != '\0') {
    int ascii = (int)toupper(text[pos]) - 65;
    for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + alphabets_size[POS_ROW]; i++) {
      for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + alphabets_size[POS_COL]; j++, k++) {
        matrix[i][j] = alphabets[ascii][i - start_position[POS_ROW]][k];
      }
    }
    start_position[1] += alphabets_size[POS_COL] + 1;
    pos++;
  }
  start_position[1] += 1;

  for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + number_font_2_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_2_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_2[before_decimal_first_digit][i - start_position[POS_ROW]][k];
    }
  }

  start_position[1] += number_font_2_size[POS_COL] + 1;
  for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + number_font_2_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_2_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_2[before_decimal_second_digit][i - start_position[POS_ROW]][k];
    }
  }

  start_position[1] += number_font_2_size[POS_COL];
  matrix[start_position[POS_ROW] + number_font_2_size[POS_ROW] - 1][start_position[1] + 1] = 1;
  start_position[1] += 3;

  for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + number_font_2_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_2_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_2[after_decimal_first_digit][i - start_position[POS_ROW]][k];
    }
  }

  start_position[1] += number_font_2_size[POS_COL] + 1;
  for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + number_font_2_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_2_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_2[after_decimal_second_digit][i - start_position[POS_ROW]][k];
    }
  }

  start_position[1] += number_font_2_size[POS_COL];

  matrix[start_position[POS_ROW] + 1][start_position[1] + 1] = 1;
  matrix[start_position[POS_ROW] + 0][start_position[1] + 1] = 1;
  matrix[start_position[POS_ROW] + 1][start_position[1] + 2] = 1;
  matrix[start_position[POS_ROW] + 0][start_position[1] + 2] = 1;
  start_position[1] += 4;

  int unit = 2;
  for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + alphabets_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + alphabets_size[POS_COL]; j++, k++) {
      matrix[i][j] = alphabets[unit][i - start_position[POS_ROW]][k];
    }
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      matrix[i][j] = graphics[SUN][i][j];
    }
  }

  return matrix;
}

int **generate_humidity(float humidity) {
  int before_decimal;
  int after_decimal;
  divide_decimal(humidity, &before_decimal, &after_decimal);
  // Allocate memory for the matrix
  int **matrix = (int **)malloc(8 * sizeof(int *));
  for (int i = 0; i < 8; i++) {
    matrix[i] = (int *)malloc(64 * sizeof(int));
  }

  int before_decimal_first_digit = 0;
  int before_decimal_second_digit = before_decimal;
  if (get_length(before_decimal) != 1) {
    before_decimal_second_digit = before_decimal % 10;
    before_decimal /= 10;
    before_decimal_first_digit = before_decimal % 10;
    before_decimal /= 10;
  }

  int after_decimal_first_digit = 0;
  int after_decimal_second_digit = after_decimal;
  if (get_length(after_decimal) != 1) {
    after_decimal_second_digit = after_decimal % 10;
    after_decimal /= 10;
    after_decimal_first_digit = after_decimal % 10;
    after_decimal /= 10;
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 64; j++) {
      matrix[i][j] = 0;
    }
  }

  int start_position[2] = { 1, 13 };

  char text[] = "HUMID";
  int pos = 0;
  while (text[pos] != '\0') {
    int ascii = (int)toupper(text[pos]) - 65;
    for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + alphabets_size[POS_ROW]; i++) {
      for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + alphabets_size[POS_COL]; j++, k++) {
        matrix[i][j] = alphabets[ascii][i - start_position[POS_ROW]][k];
      }
    }
    start_position[1] += alphabets_size[POS_COL] + 1;
    pos++;
  }
  start_position[1] += 1;

  for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + number_font_2_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_2_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_2[before_decimal_first_digit][i - start_position[POS_ROW]][k];
    }
  }

  start_position[1] += number_font_2_size[POS_COL] + 1;
  for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + number_font_2_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_2_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_2[before_decimal_second_digit][i - start_position[POS_ROW]][k];
    }
  }

  start_position[1] += number_font_2_size[POS_COL];
  matrix[start_position[POS_ROW] + number_font_2_size[POS_ROW] - 1][start_position[1] + 1] = 1;
  start_position[1] += 3;

  for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + number_font_2_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_2_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_2[after_decimal_first_digit][i - start_position[POS_ROW]][k];
    }
  }

  start_position[1] += number_font_2_size[POS_COL] + 1;
  for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + number_font_2_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_2_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_2[after_decimal_second_digit][i - start_position[POS_ROW]][k];
    }
  }

  start_position[1] += number_font_2_size[POS_COL];

  matrix[start_position[POS_ROW] + 1][start_position[1] + 1] = 1;
  matrix[start_position[POS_ROW] + 0][start_position[1] + 1] = 1;
  matrix[start_position[POS_ROW] + 1][start_position[1] + 2] = 1;
  matrix[start_position[POS_ROW] + 0][start_position[1] + 2] = 1;
  start_position[1] += 4;

  int unit = 26;
  for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + alphabets_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + alphabets_size[POS_COL]; j++, k++) {
      matrix[i][j] = alphabets[unit][i - start_position[POS_ROW]][k];
    }
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      matrix[i][j] = graphics[SUN][i][j];
    }
  }

  return matrix;
}

int **generate_time(int hour, int minutes, int second, int medium) {
  // Allocate memory for the matrix
  int **matrix = (int **)malloc(8 * sizeof(int *));
  for (int i = 0; i < 8; i++) {
    matrix[i] = (int *)malloc(64 * sizeof(int));
  }

  int hour_first_digit = 0;
  int hour_second_digit = hour;
  if (get_length(hour) != 1) {
    hour_second_digit = hour % 10;
    hour /= 10;
    hour_first_digit = hour % 10;
    hour /= 10;
  }

  int minutes_first_digit = 0;
  int minutes_second_digit = minutes;
  if (get_length(minutes) != 1) {
    minutes_second_digit = minutes % 10;
    minutes /= 10;
    minutes_first_digit = minutes % 10;
    minutes /= 10;
  }

  int second_first_digit = 0;
  int second_second_digit = second;
  if (get_length(second) != 1) {
    second_second_digit = second % 10;
    second /= 10;
    second_first_digit = second % 10;
    second /= 10;
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 64; j++) {
      matrix[i][j] = 0;
    }
  }
  int start_position[2] = { 0, 18 };
  if (medium > 0)
    start_position[1] = 14;
  // hour first digit
  for (int i = start_position[POS_ROW]; i < number_font_1_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_1_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_1[hour_first_digit][i][k];
    }
  }
  start_position[1] += number_font_1_size[POS_COL] + 1;
  // hour second digit
  for (int i = start_position[POS_ROW]; i < number_font_1_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_1_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_1[hour_second_digit][i][k];
    }
  }
  start_position[1] += number_font_1_size[POS_COL];
  matrix[1][start_position[1] + 1] = 1;
  matrix[1][start_position[1] + 2] = 1;
  matrix[2][start_position[1] + 1] = 1;
  matrix[2][start_position[1] + 2] = 1;

  matrix[4][start_position[1] + 1] = 1;
  matrix[4][start_position[1] + 2] = 1;
  matrix[5][start_position[1] + 1] = 1;
  matrix[5][start_position[1] + 2] = 1;
  start_position[1] += 4;
  // minutes first digit
  for (int i = start_position[POS_ROW]; i < number_font_1_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_1_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_1[minutes_first_digit][i][k];
    }
  }
  start_position[1] += number_font_1_size[POS_COL] + 1;
  // minutes second digit
  for (int i = start_position[POS_ROW]; i < number_font_1_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_1_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_1[minutes_second_digit][i][k];
    }
  }

  start_position[1] += number_font_1_size[POS_COL];
  matrix[1][start_position[1] + 1] = 1;
  matrix[1][start_position[1] + 2] = 1;
  matrix[2][start_position[1] + 1] = 1;
  matrix[2][start_position[1] + 2] = 1;

  matrix[4][start_position[1] + 1] = 1;
  matrix[4][start_position[1] + 2] = 1;
  matrix[5][start_position[1] + 1] = 1;
  matrix[5][start_position[1] + 2] = 1;
  start_position[1] += 4;

  // second first digit
  for (int i = start_position[POS_ROW]; i < number_font_1_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_1_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_1[second_first_digit][i][k];
    }
  }
  start_position[1] += number_font_1_size[POS_COL] + 1;
  // second first digit
  for (int i = start_position[POS_ROW]; i < number_font_1_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_1_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_1[second_second_digit][i][k];
    }
  }
  start_position[1] += number_font_1_size[POS_COL] + 2;
  start_position[0] = 2;
  if (medium = MEDIUM_AM) {
    for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + alphabets_size[POS_ROW]; i++) {
      for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + alphabets_size[POS_COL]; j++, k++) {
        matrix[i][j] = alphabets[0][i - start_position[POS_ROW]][k];
      }
    }
  } else if (medium = MEDIUM_PM) {
    for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + alphabets_size[POS_ROW]; i++) {
      for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + alphabets_size[POS_COL]; j++, k++) {
        matrix[i][j] = alphabets[15][i - start_position[POS_ROW]][k];
      }
    }
  }
  start_position[1] += alphabets_size[POS_COL] + 1;
  if (medium > 0) {
    for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + alphabets_size[POS_ROW]; i++) {
      for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + alphabets_size[POS_COL]; j++, k++) {
        matrix[i][j] = alphabets[12][i - start_position[POS_ROW]][k];
      }
    }
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      matrix[i][j] = graphics[CLOCK][i][j];
    }
  }

  return matrix;
}

int **generate_date(int dayOfWeek, int day, int month, int year) {
  char daysOfWeek[7][4] = {
    "SUN",
    "MON",
    "TUE",
    "WED",
    "THU",
    "FRI",
    "SAT"
  };
  // Allocate memory for the matrix
  int **matrix = (int **)malloc(8 * sizeof(int *));
  for (int i = 0; i < 8; i++) {
    matrix[i] = (int *)malloc(64 * sizeof(int));
  }
  char *dayName = daysOfWeek[dayOfWeek];
  int day_first_digit = 0;
  int day_second_digit = day;
  if (get_length(day) != 1) {
    day_second_digit = day % 10;
    day /= 10;
    day_first_digit = day % 10;
    day /= 10;
  }

  int month_first_digit = 0;
  int month_second_digit = month;
  if (get_length(month) != 1) {
    month_second_digit = month % 10;
    month /= 10;
    month_first_digit = month % 10;
    month /= 10;
  }

  int year_first_digit = 0;
  int year_second_digit = year;
  if (get_length(year) != 1) {
    year_second_digit = year % 10;
    year /= 10;
    year_first_digit = year % 10;
    year /= 10;
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 64; j++) {
      matrix[i][j] = 0;
    }
  }
  int start_position[2] = { 0, 13 };

  for (int i = start_position[POS_ROW]; i < number_font_1_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_1_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_1[day_first_digit][i][k];
    }
  }
  start_position[1] += number_font_1_size[POS_COL] + 1;
  for (int i = start_position[POS_ROW]; i < number_font_1_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_1_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_1[day_second_digit][i][k];
    }
  }
  start_position[1] += number_font_1_size[POS_COL];
  matrix[1][start_position[1] + 1] = 1;
  matrix[1][start_position[1] + 2] = 1;
  matrix[2][start_position[1] + 1] = 1;
  matrix[2][start_position[1] + 2] = 1;

  matrix[4][start_position[1] + 1] = 1;
  matrix[4][start_position[1] + 2] = 1;
  matrix[5][start_position[1] + 1] = 1;
  matrix[5][start_position[1] + 2] = 1;
  start_position[1] += 4;

  for (int i = start_position[POS_ROW]; i < number_font_1_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_1_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_1[month_first_digit][i][k];
    }
  }
  start_position[1] += number_font_1_size[POS_COL] + 1;

  for (int i = start_position[POS_ROW]; i < number_font_1_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_1_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_1[month_second_digit][i][k];
    }
  }

  start_position[1] += number_font_1_size[POS_COL];
  matrix[1][start_position[1] + 1] = 1;
  matrix[1][start_position[1] + 2] = 1;
  matrix[2][start_position[1] + 1] = 1;
  matrix[2][start_position[1] + 2] = 1;

  matrix[4][start_position[1] + 1] = 1;
  matrix[4][start_position[1] + 2] = 1;
  matrix[5][start_position[1] + 1] = 1;
  matrix[5][start_position[1] + 2] = 1;
  start_position[1] += 4;

  for (int i = start_position[POS_ROW]; i < number_font_1_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_1_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_1[year_first_digit][i][k];
    }
  }
  start_position[1] += number_font_1_size[POS_COL] + 1;
  for (int i = start_position[POS_ROW]; i < number_font_1_size[POS_ROW]; i++) {
    for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_1_size[POS_COL]; j++, k++) {
      matrix[i][j] = number_font_1[year_second_digit][i][k];
    }
  }

  int pos = 0;
  start_position[1] += number_font_1_size[POS_COL] + 1;
  start_position[0] = 2;
  while (dayName[pos] != '\0') {
    int ascii = (int)toupper(dayName[pos]) - 65;
    for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + alphabets_size[POS_ROW]; i++) {
      for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + alphabets_size[POS_COL]; j++, k++) {
        matrix[i][j] = alphabets[ascii][i - start_position[POS_ROW]][k];
      }
    }
    start_position[1] += alphabets_size[POS_COL] + 1;
    pos++;
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      matrix[i][j] = graphics[CALENDER][i][j];
    }
  }
  return matrix;
}

int **generate_text(int pos_row, int pos_col, const char text[]) {
  int **matrix = (int **)malloc(8 * sizeof(int *));
  for (int i = 0; i < 8; i++) {
    matrix[i] = (int *)malloc(64 * sizeof(int));
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 64; j++) {
      matrix[i][j] = 0;
    }
  }
  int start_position[2] = { pos_row, pos_col };
  int pos = 0;
  int ascii;
  while (text[pos] != '\0') {
    char myChar = text[pos];
    if (isupper(myChar)) {
      int ascii = (int)text[pos] - 65;
      for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + alphabets_size[POS_ROW]; i++) {
        for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + alphabets_size[POS_COL]; j++, k++) {
          matrix[i][j] = alphabets[ascii][i - start_position[POS_ROW]][k];
        }
      }
      start_position[1] += alphabets_size[POS_COL] + 1;
    } else if (islower(myChar)) {
      int ascii = (int)text[pos] - 97;
      for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + alphabets_small_size[POS_ROW]; i++) {
        for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + alphabets_small_size[POS_COL]; j++, k++) {
          matrix[i][j] = alphabets_small[ascii][i - start_position[POS_ROW]][k];
        }
      }
      start_position[1] += alphabets_size[POS_COL] + 1;
    } else if (isdigit(myChar)) {
      int ascii = myChar - '0';
      for (int i = start_position[POS_ROW]; i < start_position[POS_ROW] + number_font_2_size[POS_ROW]; i++) {
        for (int j = start_position[POS_COL], k = 0; j < start_position[POS_COL] + number_font_2_size[POS_COL]; j++, k++) {
          matrix[i][j] = number_font_2[ascii][i - start_position[POS_ROW]][k];
        }
      }
      start_position[1] += alphabets_size[POS_COL] + 1;
    } else {
      int ascii = (int)text[pos];
      if (ascii == 32)  // ascii of ` ` (space)
      {
        start_position[1] += 3;
      } else if (ascii == 46)  // ascii of `.` (dot)
      {
        matrix[start_position[POS_ROW] + number_font_2_size[POS_ROW] - 1][start_position[1]] = 1;
        start_position[1] += 2;
      }
    }
    pos++;
  }
  return matrix;
}

// // This function print the byte matrix 8X4 using setRow function
// // NOTE: This function also release the input matrix memory after print.
// void print_matrix(byte** matrix) {
//   for (int i = 0; i < 8; i++) {
//     for (int j = 0; j < 4; j++) {
//       lc.setRow(j, i, matrix[i][j]);
//     }
//   }
//   for (int i = 0; i < 8; i++) {
//     free(matrix[i]);
//   }
//   free(matrix);
// }
// // This function convert 8X64 matrix into 8X4 byte matrix taking 8 array element from a row
// // This function rethn the converted byte matrix.
// // NOTE: This function also release the input matrix memory after convertion.
// // WARNING: After use the function release the byte memory other wise it will leake the memory
// // eg---
// // int** matrix = generate_time(00,00,00);
// // byte** b_matrix  = conver_byte(matrix); // after this line you can not use matrix variable because conver_byte function release the matrix memory.
// // // -- Use accordingly your case for b_matrix
// // // release b_matrix
// // for (int i = 0; i < 8; i++) {
// //   free(b_matrix[i]);
// // }
// // free(b_matrix);
// byte** conver_byte(int** matrix) {
//   byte** b_arr = new byte*[8];
//   for (int i = 0; i < 8; i++) {
//     b_arr[i] = new byte[4];
//   }
//   for (int i = 0; i < 8; i++) {
//     int k = 3, j = 0;
//     String s = "";
//     while (j < 64) {
//       s += String(matrix[i][j]);
//       j++;
//       if (j % 8 == 0) {
//         b_arr[i][k] = (byte)strtol(s.c_str(), NULL, 2);
//         k--;
//       }
//     }
//   }
//   for (int i = 0; i < 8; i++) {
//     free(matrix[i]);
//   }
//   free(matrix);
//   return b_arr;
// }
