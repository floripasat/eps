#include "misc.h"

void frame_to_string(unsigned char frame[],unsigned char frame_string[], int size){
	int i;
	int j = 0;
	for(i=2; i < size; i+=5){
		frame_string[i]   = hex_to_char(frame[j] >> 4);
		frame_string[i+1] = hex_to_char(frame[j]);
		j++;
	}
}

unsigned char hex_to_char(unsigned char byte){
	char character;
	switch(byte & 0x0f){
		case 0x00: character = '0'; break;
		case 0x01: character = '1'; break;
		case 0x02: character = '2'; break;
		case 0x03: character = '3'; break;
		case 0x04: character = '4'; break;
		case 0x05: character = '5'; break;
		case 0x06: character = '6'; break;
		case 0x07: character = '7'; break;
		case 0x08: character = '8'; break;
		case 0x09: character = '9'; break;
		case 0x0A: character = 'A'; break;
		case 0x0B: character = 'B'; break;
		case 0x0C: character = 'C'; break;
		case 0x0D: character = 'D'; break;
		case 0x0E: character = 'E'; break;
		case 0x0F: character = 'F'; break;
	}
	return character;
}

void int_to_char(int data, char string[], int size){
	int i,j,n;
	n=1;
	sprintf(string, "%d", data);
	///////// >> string /////////

	//discovering the index
	for(i=0; i < size;i++){
		if (string[i]==NULL){
			j=i;
			break;
		}
	}
	//shifting to right
	for(i=j;i>=0;i--){
		string[size - n++]=string[i-1];
	}

	//filling up with blank spaces
	for(i=size - j - 1;i>=0;i--){
		string[i]=' ';
	}
	/////////(end) >> string /////////
}

uint32_t rtd_value_verification(uint32_t value1, uint32_t value2){
	uint8_t value_check = 0;
	if ((value1 > RTD_VALUE_LOWER_LIMIT) && (value1 < RTD_VALUE_UPPER_LIMIT)){
		value_check += 0x01;
	}
	if ((value2 > RTD_VALUE_LOWER_LIMIT) && (value2 < RTD_VALUE_UPPER_LIMIT)){
		value_check += 0x10;
	}
	switch(value_check){
		case 0x01: return value1;
		case 0x10: return value2;
		case 0x11: return (value1 + value2)/2;
		default: return 0x007FFFFF;
	}
	return 0x007FFFFF;
}

/*uint32_t median_value(uint32_t *vector, uint8_t size){
    int i, j;
    uint32_t temp;

    for(i=0; i<(size-1); i++) {
        for(j=i+1; j<size; j++) {
            if(vector[j] < vector[i]) {
                temp = vector[i];
                vector[i] = vector[j];
                vector[j] = temp;
            }
        }
    }
    if(size%2 == 0){
        return average(vector[size/2], vector[size/2 - 1]);
    }
    else{
        return vector[(size-1)/2];
    }
}*/
