#include<stdio.h>
#include<math.h>
#include<stdlib.h> 
#include<time.h> 

////////////////////////////////////////////////////////////////////////////////////
//  This programs gives out a fccPosition.dat file as output, which can be used   //
//  as input to "gmx insert-molecules" command to define the desired positions of //
//  the molecules in a face centered cubic (FCC) or hexagonal closeed packing     //
//  (HCP) in a cubic box. This grid will be defined as per the required number of //
//  molecules  in the box and the size of the cubic box.                          //
////////////////////////////////////////////////////////////////////////////////////


int findCeil(int arr[], int r, int l, int h)  
{  
    int mid;  
    while (l < h)  
    {  
        mid = l + ((h - l) >> 1); // Same as mid = (l+h)/2  
        (r > arr[mid]) ? (l = mid + 1) : (h = mid);  
    }  
    return (arr[l] >= r) ? l : -1;  
}  
  
// The main function that returns a random number 
// from arr[] according to distribution array  
// defined by freq[]. n is size of arrays.  
int myRand(int arr[], int freq[], int n)  
{  
    // Create and fill prefix array  
    int prefix[n], i;  
    prefix[0] = freq[0];  
    for (i = 1; i < n; ++i)  
        prefix[i] = prefix[i - 1] + freq[i];  
  
    // prefix[n-1] is sum of all frequencies. 
    // Generate a random number with  
    // value from 1 to this sum  
    int r = (rand() % prefix[n - 1]) + 1;  
  
    // Find index of ceiling of r in prefix arrat  
    int indexc = findCeil(prefix, r, 0, n - 1);  
    return arr[indexc];  
}  

int main()
{
        float a, b, l;
        int n, nl;

        b = 10;					// size of the simulation box in nm
	printf("Enter the size of the box (nm):");
	scanf("%f",&b);
        n = 500;				// number of molecules to be put in simulation box
	printf("Enter the number of molecules required:");
	scanf("%d",&n);
        l = cbrt(n/4);                                 
	nl = ceil(l);				// number of molecules in each row or column
        a = b/nl;				// length of a unit cell in nm      
	printf("Length of one unit cell: %f\nNumber of molecules in each row: %d\n",a,nl);

        FILE * fp;

        fp = fopen("fccPosition.dat","w");
        fprintf(fp,"#       X          Y          Z\n");

        float i, j, k;
	int icount, jcount, kcount;

	k = 0;

	for(kcount=0; kcount<nl; kcount++){

                // LAYER A
		if(k<b){
			j = 0; 
                	for(jcount=0; jcount<nl; jcount++){
				i = 0;
                        	for(icount=0; icount<nl; icount++){
					if(i<b && j<b){
                                		fprintf(fp,"%9.4f  %9.4f  %9.4f\n", i, j, k);
					}
                                	if(i+a/2<b && j+a/2<b){
                                        	fprintf(fp,"%9.4f  %9.4f  %9.4f\n", i+a/2, j+a/2, k);
                                	}
					i = i+a;
                        	}
				j = j+a;
                	}
               // fprintf(fp,"\n\n");
		}

                // LAYER B
		if(k+a/2 < b){
			j = 0;
                	for(jcount=0; jcount<nl; jcount++){
				i = 0;
                        	for(icount=0; icount<nl; icount++){
                                	if(i+a/2<b && j+a/2<b && k+a/2<b){
                                        	fprintf(fp,"%9.4f  %9.4f  %9.4f\n", i, j+a/2, k+a/2);
                                        	fprintf(fp,"%9.4f  %9.4f  %9.4f\n", i+a/2, j, k+a/2);
                                	}
					i = i+a;
                        	}
				j = j+a;
                	}
			k = k+a;
                	// fprintf(fp,"\n\n\n\n");
		}
        }

        // Layers A and B keep on repeating in Z-direction

        fclose(fp);

        // Removing some of the coordinates so that we only have the required number

        int rmvRow, totRow;
        int rmvCount = 0;
		totRow = nl*nl*nl*4;
        rmvRow = totRow - n;
		int arr[]={1,0};
		int freq[]={rmvRow,n};
		srand(time(NULL));

		// printf("rmvRow = %d\n",rmvRow);
		// printf("Total row = %d\n", totRow);

        FILE *temp;

        char row[36];

        fp = fopen("fccPosition.dat","r");
        temp = fopen("temp.txt","w");

        while(1){
            if(fgets(row,36,fp)!=NULL){
                if(rmvCount<rmvRow){
					if(myRand(arr, freq, 2)==1){
                    	fputc('#',temp);
                    	rmvCount++;
               		}
				}
                fputs(row,temp);
            }
            if(feof(fp)){
                break;
            }
        }

        fclose(fp);
        fclose(temp);

        remove("fccPosition.dat");
        rename("temp.txt","fccPosition.dat");

        return 0;
}

