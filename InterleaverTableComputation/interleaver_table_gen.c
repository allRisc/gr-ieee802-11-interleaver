/*
 * Copyright (C) 2013 Bastian Bloessl <bloessl@ccs-labs.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "hash_drgb.h"

#include <stdint.h>
#include <stdio.h>

uint8_t SHARED_SECRET[] = {0xF3, 0x9D, 0x59, 0x9C};
uint8_t SHARED_SECRET_SIZE = 4;

/*void interleave(int* in, int* out, */

int* interleave_gen(int n_bpsc, int num_subcarriers);

int main()
{
    int bpsc[] = {1, 2, 4, 6, 8};
    int i, j;
    int* dscShiftArr;

    for (i = 0; i < 5; i++)
    {
        dscShiftArr = interleave_gen(bpsc[i], 48);
        printf("if (ofdm.n_bpsc == %d) \n{\n", bpsc[i]);
        printf("\tint temp[] = {");

        for (j = 0; j < bpsc[i]; j++)
        {
            printf("%d", dscShiftArr[j]);
            if (j < (bpsc[i] - 1))
            {
                printf(", ");
            }
        }
        printf("};\n");

        printf("\tfor (i = 0; i < ofdm.n_bpsc; i++)\n\t{\n");
        printf("\t\tdscShiftArr[i] = temp[i];\n");
        printf("\t}\n");
        printf("}\n");
    }

    return 0;
}



int* interleave_gen(int n_bpsc, int num_subcarriers) 
{
	Hash_DRGB drgb;
	int i, j;
	int temp;
	uint8_t contains;
	uint8_t drgbBuffer[8];
	uint64_t temp64;
	double tempDouble;
    static int dscShiftArr[8];

	/* Initialize deterministic random number generator */
	hash_drgb_init(&drgb, SHARED_SECRET, SHARED_SECRET_SIZE, NULL, 0, NULL, 0);

	/* Determine which dsc shifts will be used w/ no repetitions */
	i = 0; 
	while (i < n_bpsc)
	{
		hash_drgb_get_rand(&drgb,
						   drgbBuffer, 8,
						   NULL, 0);
		
		temp64 = *((uint64_t*)drgbBuffer);
		tempDouble = (double)temp64 / (double)0xFFFFFFFFFFFFFFFF;
		temp = (int)(tempDouble *48);

		contains = 0;
		for (j = 0; j < i; j++)
		{
			if (dscShiftArr[j] == temp)
			{
				contains = 1;
				break;
			}
		}
		if (!contains)
		{
			dscShiftArr[i] = temp;
			i++;
		}
	}

	for (i = 0; i < n_bpsc; i++)
	{
		hash_drgb_get_rand(&drgb,
                           drgbBuffer, 4,
                           NULL, 0);

        j = drgbBuffer[1] % (n_bpsc - i) + i; 

        temp = dscShiftArr[j];
        dscShiftArr[j] = dscShiftArr[i];
        dscShiftArr[i] = temp; 
	}	


    return dscShiftArr;
}


