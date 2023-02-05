struct sValues
{
	int current;
	int current_digit_count;
	int min_range;
	int min_range_digit_count;
	int max_range;
	int max_range_digit_count;
	int safety_margin;
	int current_ready;
};
typedef struct sValues Values;

int getDigitCount(int number);

void setRange(Values *vals, int min, int range);

int calculateAverage(int *arr, int len);

// global struct instance that holds all the data
static Values vals_;

Values *getValues(void);

void initValues(void);
