// selection sort algorithm 

public class w01 {
    static int[] sort(int[] array) {
        // finding the smallest value
        int min = array[0];
        int index = 0;
        int k;
        for (k = 0; k < array.length; k++) {
            if (array[k] < min) {
                min = array[k];
                index = k;
            }
        }

        // swapping the smallest array with the array[0] so the smallest can be first
        int temp = array[0];
        array[0] = array[index];
        array[index] = temp;

        int i;
        int j;
        for (i = 1; i < array.length; i++) {
            int small = array[i];
            for (j = i + 1; j < array.length; j++) {
                if (array[j] < small) {
                    small = array[j];
                }
                int tempo = array[i];
                array[i] = small;
                array[j] = tempo;
            }



        }
        
        return array;
    }

    public static void main(String[] args) {
        int[] array = {8, 34, 14, 2};
        int[] sorted = sort(array);
        
        // Expected result: 2, 8, 14, 34[]
        for (int num : sorted) {
            System.out.println(num);
        }
    }
}
