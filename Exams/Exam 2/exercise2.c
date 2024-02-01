#include <stdint.h>
#include <stdio.h>

#define D 200
//////////////////////////////////////////////////////////////////////////////////
// TODO: Write down your DNI without the final letter
//////////////////////////////////////////////////////////////////////////////////
const unsigned int DNI =  71789634 ; //Example: const unsigned int DNI = 12345678

void fillMatrix1(uint8_t matrix[][D][D]) {
    for (int i = 0; i < D; i++) {
        for (int j = 0; j < D; j++) {
            for (int k = 0; k < D; k++) {
                matrix[i][j][k] = 0;
            }
        }
    }
}

void fillMatrix2(uint8_t matrix[][D][D]) {
    for (int k = 0; k < D; k++) {
        for (int i = 0; i < D; i++) {
            for (int j = 0; j < D; j++) {
                matrix[i][j][k] = 0;
            }
        }
    }
}

void fillMatrix3(uint8_t matrix[][D][D]) {
    for (int j = 0; j < D; j++) {
        for (int k = 0; k < D; k++) {
            for (int i = 0; i < D; i++) {
                matrix[i][j][k] = 0;
            }
        }
    }
}

void fillMatrix4(uint8_t matrix[][D][D]) {
    for (int k = 0; k < D; k++) {
        for (int j = 0; j < D; j++) {
            for (int i = 0; i < D; i++) {
                matrix[i][j][k] = 0;
            }
        }
    }
}

void fillMatrix5(uint8_t matrix[][D][D]) {
    for (int i = 0; i < D; i++) {
        for (int k = 0; k < D; k++) {
            for (int j = 0; j < D; j++) {
                matrix[i][j][k] = 0;
            }
        }
    }
}

void fillMatrix6(uint8_t matrix[][D][D]) {
    for (int j = 0; j < D; j++) {
        for (int i = 0; i < D; i++) {
            for (int k = 0; k < D; k++) {
                matrix[i][j][k] = 0;
            }
        }
    }
}




int main() {
    uint8_t matrix[D][D][D];
    int number = DNI % 6;

    for (int iteration = 0; iteration < 20; iteration++){
    	switch (number) {
        	case 0:
	            fillMatrix1(matrix);
	            break;
	        case 1:
	            fillMatrix2(matrix);
	            break;
	        case 2:
	            fillMatrix3(matrix);
	            break;
	        case 3:
	            fillMatrix4(matrix);
	            break;
	        case 4:
	            fillMatrix5(matrix);
	            break;
	        case 5:
	            fillMatrix6(matrix);
	            break;
	        default:
	            printf("Code error.\n");
	    }
    }

    return 0;
}
