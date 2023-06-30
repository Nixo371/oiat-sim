#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

#define MAX_SAUCE 10000000000

long int	rarities[20] = {500, 1000, 2000, 5000, 10000, 25000, 50000, 75000, 100000, 250000, 500000, 750000, 1000000, 5000000, 10000000, 100000000, 1000000000, 10000000000, 100000000000, 1000000000000};
long int	sell_value[20] = {750, 1250, 2500, 6250, 12000, 30000, 60000, 90000, 100000, 250000, 500000, 750000, 1000000, 4500000, 9000000, 80000000, 700000000, 6000000000, 50000000000, 400000000000};

// largest value in array
long int	get_max1(long int *list)
{
	long int max = 0;
	int	j;

	for (int i = 0; i < 11; i++)
	{
		if (max < list[i])
		{
			max = list[i];
			j = i;
		}
	}
	return (j);
}

// second largest value in array
long int	get_max2(long int *list)
{
	long int max = 0;
	int	j;

	for (int i = 0; i < 11; i++)
	{
		if (max < list[i])
		{
			max = list[i];
			j = i;
		}
			
	}
	if (list[j + 1] > list[j - 1])
		j++;
	else
		j--;
	return (j);
}

// gets 2 32-bit random numbers and just kinda squishes them together
long int	get_rand()
{
	long int	random_left;
	long int	random_right;

	random_left = rand();
	random_left <<= 32;
	random_right = rand();
	return (random_left | random_right);
}

// uses a single random number to check each egg rarities' probability of happening
long int	simulate_clean(int sauce, long int rarity[20])
{
	long int max_rarity = rarity[0];
	long int money = sell_value[0];
	long int r;
	long int r_temp;
	
	for (int i = 0; i < sauce; i++)
	{
		r = get_rand();
		for (int j = 19; j >= 0; j--)
		{
			// for example if we divide by 10k, the 500 eggs would be 0
			if (rarity[j] == 0)
				break;
			//	don't want to check for eggs we already have/better
			if (rarity[j] <= max_rarity)
				break;
			r_temp = r % rarity[j];
			if (r_temp == 0)
			{
				max_rarity = rarity[j];
				money = sell_value[j];
				//printf("Got 1 in %ld! Sell value: %ld\n", rarity[j], sell_value[j]);
			}
		}
	}
	return (money);
}

long int	simulate_cleans(long int sauce, int amount)
{
	long int	new_sauce = sauce;
	int			multiplier = 1;
	long int	new_rarities[20];
	long int	profit = 0;
	long int	sauce_cost = sauce * 1.2;

	// calculates new "simulated" sauce
	while (new_sauce > 100000)
	{
		new_sauce = new_sauce / 10;
		multiplier = multiplier * 10;
	}
	// creates new rarities array based off new sauce
	for (int i = 0; i < 20; i++)
	{
		new_rarities[i] = rarities[i] / multiplier;
	}
	// simulates each egg
	for (int i = 0; i < amount; i++)
	{
		long int 	sim_result = simulate_clean(new_sauce, new_rarities);
		long int	net_profit = sim_result - sauce_cost;
		//printf("Money Gotten: $%ld\n", sim_result);
		//printf("Cost of Luck Sauce: $%ld\n", sauce_cost);
		//printf("Net Profit = %ld\n", net_profit);
		profit += net_profit;
		//printf("Profit #%d: $%ld\n", i, simulate_clean(new_sauce, new_rarities) - (int)(sauce * 1.2));
	}
	printf("Expected profit for eggs with %ld sauce: $%ld\n", sauce, profit / amount);
	return (profit / amount);
}

// runs recursively through a range making its range 10 times smaller each iteration
// 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100
// turns to...
// 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40
long int	calculate_best_sauce(long int offset, long int min, long int max)
{
	long int	expected_value[11];
	long int	sauce = min;

	if (offset == 0)
		return (min);
	for (int i = 0; i < 11; i++)
	{
		expected_value[i] = simulate_cleans(min + (offset * i), 1000);
	}
	// this whole thing is to figure out which of the 2 values (max1 and max2)
	// is supposed to be the smaller one in our list
	int max1 = get_max1(expected_value);
	int max2 = get_max2(expected_value);
	long int	min_sauce;
	long int	max_sauce;
	if (max1 > max2)
	{
		min_sauce = min + (offset * max2);
		max_sauce = min + (offset * max1);
	}
	else
	{
		min_sauce = min + (offset * max1);
		max_sauce = min + (offset * max2);
	}
	return (calculate_best_sauce(offset / 10, min_sauce, max_sauce));
}

int main()
{
	long int	offset = 1000000000;
	long int	best_sauce;

	srand(time(NULL));
	//best_sauce = simulate_cleans(80000, 1000);
	//best_sauce = calculate_best_sauce(offset, 0, MAX_SAUCE);	**
	//printf("Best Sauce: %ld\n", best_sauce);					**

	simulate_cleans(2000000000, 1000000);

	//int eggs_less_rare = 0;
	/*for (int i = 0; i < 1000; i++)
	{
		printf("#%d: $%ld\n", i, simulate_clean(2000000000, rarities));
		//if (simulate_clean(100000, rarities) < 100000)
			//eggs_less_rare++;
	}*/
	//printf("Eggs less rare: %d\n", eggs_less_rare);
	
}
// right now the main function just has the simulate cleans one so its only doing 1 sim
// for the full thing you gotta uncomment the lines I marked with an asterisk (**)
