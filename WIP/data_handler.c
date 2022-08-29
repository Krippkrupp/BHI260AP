#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"

#define FORMAT_QUAT 1
#define FORMAT_EULER 2
#define FORMAT_3DVECT 3

typedef struct quaternion{
    uint16_t x, y, z, w;
    uint16_t accuracy;
}quaternion_t;

typedef struct euler{
    uint16_t heading, pitch, roll;
}euler_t;


typedef struct vect_3d{
    uint16_t x, y, z;
}vect_3d_t;
// typedef union data_type{
//     struct quaternion_t;
//     struct euler_t;
// }data_type_t;

typedef struct fifo_data{
    uint8_t event, format;
    uint32_t time;
    // data_type_t data;
    union {
        quaternion_t quat;
        euler_t eul;
        vect_3d_t vect;
    };
} fifo_data_t;


void print_fifo(fifo_data_t fifo)
{
    switch(fifo.format)
    {
        case FORMAT_QUAT:
        //
			// printf("5/3 = %f", (float)5/3);
            printf("Quaternion: Event: %u, Time ticks: %d, Real time: %f, X=%u, Y=%u, Z=%u, W=%u\n", fifo.event, fifo.time, (float)fifo.time/64000, fifo.quat.x, fifo.quat.y, fifo.quat.z, fifo.quat.w);
        break;

        case FORMAT_EULER:
        //
            printf("Euler: Event: %u, Time ticks: %d, Real time: %f , Heading=%u, Pitch=%u, Roll=%u\n", fifo.event, fifo.time, fifo.eul.heading, fifo.eul.pitch, fifo.eul.roll);
        break;
        
        case FORMAT_3DVECT:
        //
            printf("3D Vector: Event: %u, Time ticks: %d, Real time: %f, X=%u, Y=%u, Z=%u\n", fifo.event, fifo.time, fifo.vect.x, fifo.vect.y, fifo.vect.z);
        break;
        
        default:
        printf("Unknown format!\n");
    }
 
}

#define TIMESTAMP_SMALL_DELTA_NWU   	0xfb
#define TIMESTAMP_LARGE_DELTA_NWU   	0xfc
#define TIMESTAMP_FULL_NWU          	0xfd
#define META_EVENT_NWU              	0xfe

#define TIMESTAMP_SMALL_DELTA_WU   	0xf5
#define TIMESTAMP_LARGE_DELTA_WU   	0xf6
#define TIMESTAMP_FULL_WU          	0xf7
#define META_EVENT_WU              	0xf8


#define ROTATION_VECT_NWU           		34
#define ROTATION_VECT_WU            		35
#define GAME_ROTATION_VECT_NWU          	37
#define GAME_ROTATION_VECT_WU           	38
#define GEOMAGNETIC_ROTATION_VECTOR_NWE 	40
#define GEOMAGNETIC_ROTATION_VECTOR_WE  	41
#define ORIENTATION_NWU						43
#define ORIENTATION_WU						44
#define FILLER							0x00

void handle_msg()
{
    // uint16_t RxFifoEndIndex = 0x36;
    // uint8_t RxBuffer[] = {0xfb, 0x0, 0xfe, 0x14, 0x3, 0x0, 0xfd, 0x4e, 0x3c, 0x18, 0x0, 0x0, 0x25, 0x5f, 0x4, 0x1d, 0xff, 0x18, 0x0, 0xd8, 0x3f, 0x0, 0x0, 0xfc, 0xd8, 0xa1, 0x25,0x60, 0x4, 0x44, 0xff, 0x10, 0x0, 0xd8, 0x3f, 0x0 ,0x0 ,0xfc, 0xdb ,0xa1, 0x25, 0x60, 0x4,0x6e, 0xff, 0xb, 0x0, 0xd8, 0x3f, 0x0, 0x0, 0x0, 0x0, 0x0};
    
	// uint16_t RxFifoEndIndex = 54;
    // uint8_t RxBuffer[] = {0xfb, 0x0, 0xfe, 0x14, 0x13, 0x0, 0xfd, 0x96, 0xf1, 0x9c, 0x7, 0x0, 0x25, 0x57, 0x4, 0x91, 0xff, 0xff, 0xff, 0xd9, 0x3f, 0x0, 0x0, 0xfc, 0xce, 0xa1, 0x25, 0x59, 0x4, 0x94, 0xff, 0xff, 0xff, 0xd9, 0x3f, 0x0, 0x0, 0xfc, 0xd1, 0xa1, 0x25, 0x59, 0x4, 0x91, 0xff, 0xff, 0xff, 0xd9, 0x3f, 0x0, 0x0, 0x0, 0x0, 0x0};

	uint16_t RxFifoEndIndex = 18;
    uint8_t RxBuffer[] = {0xfb, 0x0, 0xfe, 0x14, 0x1, 0x0, 0xfd, 0x35, 0x1d, 0x0, 0x0, 0x0, 0xfe, 0x10, 0x67, 0x17, 0x0, 0x0};
    // uint16_t len = ( RxBuffer[1] << 8) | ( RxBuffer[0] );
    // printf("len = %u\n", len);

    uint16_t RxFifoStartIndex = 0;

    uint64_t base_time = 0;
    uint8_t small_delta_time = 0;  // Incremental change from previous
    uint16_t large_delta_time = 0;  //  Invremental change from previous base_time(?)
    uint64_t current_time = 0;
    
	// fifo_data_t *fifo_ret = NULL;	//	Doesn't work unless this is NULL, for some unknown reason
	fifo_data_t *fifo_ret = NULL;	//	Doesn't work unless this is NULL, for some unknown reason
	fifo_data_t *tmp_ptr;
	uint8_t fifo_len = 0;

	
    while(RxFifoStartIndex < RxFifoEndIndex)
    {
		printf("index = %d\n", RxFifoStartIndex);
        switch(RxBuffer[RxFifoStartIndex])
        {
			case FILLER:
				RxFifoStartIndex++;
			break;

            case TIMESTAMP_SMALL_DELTA_WU: // Length = 2
            case TIMESTAMP_SMALL_DELTA_NWU: // Length = 2
                // This is never used?
                small_delta_time = RxBuffer[RxFifoStartIndex+1];
                RxFifoStartIndex += 2;
            break;

            case TIMESTAMP_LARGE_DELTA_WU: //  Length = 3
            case TIMESTAMP_LARGE_DELTA_NWU: //  Length = 3
                //  This seems to be used in each header following the first one. 
                //  Note that the datasheet is entirely wrong when it comes to FIFO packets, this is a lot of guesswork to try and fit everything together
                large_delta_time = ( RxBuffer[RxFifoStartIndex+2] << 8*1 ) | ( RxBuffer[RxFifoStartIndex+1] << 8*0 );
                RxFifoStartIndex+=3;
                current_time += large_delta_time;
            break;

            case TIMESTAMP_FULL_WU:    //  Length = 6
            case TIMESTAMP_FULL_NWU:    //  Length = 6
                base_time  = ( RxBuffer[RxFifoStartIndex+5] << 8*4 ) | ( RxBuffer[RxFifoStartIndex+4] << 8*3 ) | ( RxBuffer[RxFifoStartIndex+3] << 8*2 ) | ( RxBuffer[RxFifoStartIndex+2] << 8*1 ) | ( RxBuffer[RxFifoStartIndex+1] << 8*0 );
                // base_time  = ( RxBuffer[RxFifoStartIndex+5] << 8*4 );
                RxFifoStartIndex+=6;
                current_time = base_time;
            break;

            case META_EVENT_WU:    //  Length = 4?
            case META_EVENT_NWU:    //  Length = 4?
                // TODO: IS THIS ALWAYS 4? According to Table 88, it should be 4
                // We don't need to handle meta event
                RxFifoStartIndex += 4;
            break;

            // case 34 | 
            case ROTATION_VECT_NWU:					//34: 
            case ROTATION_VECT_WU:					//35: 
            case GAME_ROTATION_VECT_NWU:			//37: 	
            case GAME_ROTATION_VECT_WU:				//38: 
            case GEOMAGNETIC_ROTATION_VECTOR_NWE:	//40: 	
            case GEOMAGNETIC_ROTATION_VECTOR_WE:	//41: 	
				printf("Entered quat \n");
				//	Length 11
					//	Reallocate memory				
				tmp_ptr = realloc(fifo_ret, (++fifo_len)*(sizeof(fifo_data_t)));
					// Handle sensor data for Quaternions+
				tmp_ptr[fifo_len-1].event = RxBuffer[RxFifoStartIndex];
				tmp_ptr[fifo_len-1].time = current_time;
				tmp_ptr[fifo_len-1].format = FORMAT_QUAT;
				tmp_ptr[fifo_len-1].quat.x = ( RxBuffer[RxFifoStartIndex+2] << 8 ) | ( RxBuffer[RxFifoStartIndex+1] );
				tmp_ptr[fifo_len-1].quat.y = ( RxBuffer[RxFifoStartIndex+4] << 8 ) | ( RxBuffer[RxFifoStartIndex+3] );
				tmp_ptr[fifo_len-1].quat.z = ( RxBuffer[RxFifoStartIndex+6] << 8 ) | ( RxBuffer[RxFifoStartIndex+5] );
				tmp_ptr[fifo_len-1].quat.w = ( RxBuffer[RxFifoStartIndex+8] << 8 ) | ( RxBuffer[RxFifoStartIndex+7] );
				tmp_ptr[fifo_len-1].quat.accuracy = ( RxBuffer[RxFifoStartIndex+10] << 8 ) | ( RxBuffer[RxFifoStartIndex+9] );
					//	Set fifo_ret pointer to tmp_ptr
				fifo_ret = tmp_ptr;
				
				// Handle quaternions
				// (Sensor ID) = RxBuffer[RxFifoStartIndex]
				// x = ( RxBuffer[RxFifoStartIndex+2] << 8 ) | ( RxBuffer[RxFifoStartIndex+1] );
				// y = ( RxBuffer[RxFifoStartIndex+4] << 8 ) | ( RxBuffer[RxFifoStartIndex+3] );
				// z = ( RxBuffer[RxFifoStartIndex+6] << 8 ) | ( RxBuffer[RxFifoStartIndex+5] );
				// w = ( RxBuffer[RxFifoStartIndex+8] << 8 ) | ( RxBuffer[RxFifoStartIndex+7] );
				// accuracy = ( RxBuffer[RxFifoStartIndex+8] << 10 ) | ( RxBuffer[RxFifoStartIndex+9] );
				RxFifoStartIndex += 11;
				// printf("Exited quat \n");
			break;

			case ORIENTATION_NWU: //43: // 
			case ORIENTATION_WU: //44: // 
				// EULER
				//	Length 11
				// Handle quaternions
				// (Sensor ID) = RxBuffer[RxFifoStartIndex]
				// heading = ( RxBuffer[RxFifoStartIndex+2] << 8 ) | ( RxBuffer[RxFifoStartIndex+1] );
				// pitch = ( RxBuffer[RxFifoStartIndex+4] << 8 ) | ( RxBuffer[RxFifoStartIndex+3] );
				// roll = ( RxBuffer[RxFifoStartIndex+6] << 8 ) | ( RxBuffer[RxFifoStartIndex+5] );

					// arrlen +=1;
				tmp_ptr = realloc(fifo_ret, (++fifo_len)*(sizeof(fifo_data_t)));
				tmp_ptr[fifo_len].event = RxBuffer[RxFifoStartIndex];
				tmp_ptr[fifo_len].time = current_time;
				tmp_ptr[fifo_len].format = FORMAT_EULER;
				tmp_ptr[fifo_len].eul.heading = ( RxBuffer[RxFifoStartIndex+2] << 8 ) | ( RxBuffer[RxFifoStartIndex+1] );
				tmp_ptr[fifo_len].eul.pitch = ( RxBuffer[RxFifoStartIndex+4] << 8 ) | ( RxBuffer[RxFifoStartIndex+3] );
				tmp_ptr[fifo_len].eul.roll = ( RxBuffer[RxFifoStartIndex+6] << 8 ) | ( RxBuffer[RxFifoStartIndex+5] );
				// print_fifo(tmp_ptr[0]);
				fifo_ret = tmp_ptr;
			break;


			case 1:		//	ACCELEROMETER_PASSTHROUGH_NWU
			case 10:	//	GYROSCOPE_PASSTHROUGH_NWU
			case 19:	//	MAGNETOMETER_PASSTHROUGH_NWU
			case 4:		//	ACCELEROMETER_CORRECT_NWU
			case 6:		//	ACCELEROMETER_CORRECT_WU
			case 22:	//	MAGNETOMETER_CORRECTED_NWU
			case 24:	//	MAGNETOMETER_CORRECTED_WU
			case 13:	//	GYROSCOPE_CORRECTED_NWU
			case 15:	//	GYROSCOPE_CORRECTED_WU
			case 28:	//	GRAVITY_NWU
			case 29:	//	GRAVITY_WU
			case 31:	//	LINEAR_ACCELERATION_NWU
			case 32:	//	LINEAR_ACCELERATION_WU
			case 3:		//	RAW_ACCELEROMETER_NWU
			case 7:		//	RAW_ACCELEROMETER_WU		
			case 21:	//	RAW_MAGNETOMETER_NWU
			case 25:	//	RAW_MAGNETOMETER_WU
			case 12:	//	RAW_GYROSCOPE_NWU
			case 16:	//	RAW_GYROSCOPE_WU
			case 5:		//	OFFSET_ACCELEROMETER_NWU
			case 91:	//	OFFSET_ACCELEROMETER_WU
			case 23:	//	OFFSET_MAGNETOMETER_NWU
			case 93:	//	OFFSET_MAGNETOMETER_WU
			case 14:	//	OFFSET_GYROSCOPE_NWU
			case 92:	//	OFFSET_GYROSCOPE_WU
				// 3d vector, length = 7
				// sensor = RxBuffer[RxFifoStartIndex]
				// x = ( RxBuffer[RxFifoStartIndex+2] << 8 ) | ( RxBuffer[RxFifoStartIndex+1] );
				// y = ( RxBuffer[RxFifoStartIndex+4] << 8 ) | ( RxBuffer[RxFifoStartIndex+3] );
				// z = ( RxBuffer[RxFifoStartIndex+6] << 8 ) | ( RxBuffer[RxFifoStartIndex+5] );
			break;

            default:
                printf("I'm not sure WHY but i'm sure THAT something went wrong");
        }
    }
	for(int i = 0; i < fifo_len; i++)
	{
		print_fifo(fifo_ret[i]);
	}
}

int main(int argc, char *argv[]){
    printf("Hello World!\n");
    // fifo_data_t test;
    // test.event = 1;
	// test.format = 1;
	// test.time = 5;
    // test.quat.x=10;
    // test.quat.y=20;
    // test.quat.z=30;
    // test.quat.w=40;
	// print_fifo(test);

	// fifo_data_t *tmp_ptr;
	// fifo_data_t *fifo_ret;
	// uint8_t fifo_len = 0;
	// fifo_len+=1;
	// tmp_ptr = realloc(fifo_ret, (fifo_len)*(sizeof(fifo_data_t)));
	// // tmp_ptr = realloc(fifo_ret, (++fifo_len)*(sizeof(fifo_data_t)));
	// printf("1: \n");
	// printf("fifo_len = %d \n", fifo_len);
	// tmp_ptr[fifo_len-1].event = 1;
	// tmp_ptr[fifo_len-1].time = 0;
	// tmp_ptr[fifo_len-1].format = FORMAT_QUAT;
	// printf("2: \n");
	// tmp_ptr[fifo_len-1].quat.x = 1;
	// tmp_ptr[fifo_len-1].quat.y = 2;
	// tmp_ptr[fifo_len-1].quat.z = 3;
	// tmp_ptr[fifo_len-1].quat.w = 4;
	// tmp_ptr[fifo_len-1].quat.accuracy = 5;
	// printf("1.5: \n");
	// // tmp_ptr[fifo_len-1].event = RxBuffer[RxFifoStartIndex];
	// // tmp_ptr[fifo_len-1].time = current_time;
	// // tmp_ptr[fifo_len-1].format = FORMAT_QUAT;
	// // printf("2: \n");
	// // tmp_ptr[fifo_len-1].quat.x = ( RxBuffer[RxFifoStartIndex+2] << 8 ) | ( RxBuffer[RxFifoStartIndex+1] );
	// // tmp_ptr[fifo_len-1].quat.y = ( RxBuffer[RxFifoStartIndex+4] << 8 ) | ( RxBuffer[RxFifoStartIndex+3] );
	// // tmp_ptr[fifo_len-1].quat.z = ( RxBuffer[RxFifoStartIndex+6] << 8 ) | ( RxBuffer[RxFifoStartIndex+5] );
	// // tmp_ptr[fifo_len-1].quat.w = ( RxBuffer[RxFifoStartIndex+8] << 8 ) | ( RxBuffer[RxFifoStartIndex+7] );
	// // tmp_ptr[fifo_len-1].quat.accuracy = ( RxBuffer[RxFifoStartIndex+10] << 8 ) | ( RxBuffer[RxFifoStartIndex+9] );
	// printf("3: \n");
	// fifo_ret = tmp_ptr;
	// print_fifo(fifo_ret[0]);

	handle_msg();
/*
	fifo_data_t *fifoarr;
	fifo_data_t *tmp_ptr;
	uint8_t arrlen = 0;
	
	// arrlen +=1;
	tmp_ptr = realloc(fifoarr, (++arrlen)*(sizeof(fifo_data_t)));
	// tmp_ptr = realloc(fifoarr, (arrlen++)*(sizeof(fifo_data_t)));
	tmp_ptr[0].event = 1;
	tmp_ptr[0].time = 1;
	tmp_ptr[0].format = FORMAT_QUAT;
	tmp_ptr[0].quat.x = 10;
	tmp_ptr[0].quat.y = 20;
	tmp_ptr[0].quat.z = 30;
	// print_fifo(tmp_ptr[0]);
	fifoarr = tmp_ptr;
	print_fifo(fifoarr[0]);	

	// arrlen +=1;
	tmp_ptr = realloc(fifoarr, (++arrlen)*(sizeof(fifo_data_t)));
	tmp_ptr[1].event = 2;
	tmp_ptr[1].time = 2;
	tmp_ptr[1].format = FORMAT_EULER;
	tmp_ptr[1].eul.heading = 5;
	tmp_ptr[1].eul.pitch = 0;
	tmp_ptr[1].eul.roll = 3;
	// print_fifo(tmp_ptr[0]);
	fifoarr = tmp_ptr;
	printf("Reallocated memory, printing again!\n");
	print_fifo(fifoarr[0]);	
	print_fifo(fifoarr[1]);	*/



	// char *t = calloc(5, sizeof(char));
	// t[0] = 'A';
	// t[1] = 'B';
	// t[2] = 'C';
	// t[3] = 'D';
	// t[4] = 'E';
	// t[5] = 'F';
	// for(int i = 0; i < 7; i++)
	// {
	// 	printf("%c ", t[i]);
	// }
	// realloc(t, 7*sizeof(char));
	// printf("\nReallocating!\n");
	// t[6] = 'G';
	// t[7] = 'H';
	// for(int i = 0; i < 7; i++)
	// {
	// 	printf("%c ", t[i]);
	// }

	
    // int a = 5;
    // switch(a)
    // {
    //     case 4:
    //         printf("4!\n");
    //     break;
    //     case 6: 
    //     case 5:
    //         printf("5 or 6!\n");
    //     break;
    //     default:
    //         printf("dunno");
    // }

    printf("Exiting!\n");
    return 0;
}

