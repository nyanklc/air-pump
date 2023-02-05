#include "svalues.h"

void initValues(void)
{
	int i = 0;
	
	vals_.current = 0;
	vals_.current_digit_count = 0;
	vals_.min_range = 0;
	vals_.min_range_digit_count = 0;
	vals_.max_range = 0;
	vals_.max_range_digit_count = 0;
	
//	for(i = 0; i < 256; i++)
//	{
//		vals_.samples[i] = 0;
//	}
	vals_.current_ready = 0;
	
	return;
}

Values *getValues(void)
{
	return &vals_;
}

int getDigitCount(int number)
{
	int count = 0;
   while(number != 0) {
      number = number / 10;
      count++;
   }
   return count;
}

void setRange(Values *vals, int min, int range)
{
	vals->min_range = min;
	vals->min_range_digit_count = getDigitCount(min);
	vals->max_range = min + range;
	vals->max_range_digit_count = getDigitCount(vals->max_range);
}

int calculateAverage(int *arr, int len)
{
	int i = 0;
	int sum = 0;
	for (i = 0; i < len; i++)
	{
		sum+=arr[i];
	}
	return sum/len;
}
