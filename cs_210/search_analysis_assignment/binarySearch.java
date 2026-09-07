// Linear Search

public class binarySearch {
    public static void main(String[] args) {
        int[] numList = {2, 45, 3, 7, 21, 33};
        int target = 7;
        int index = 0;
        for (int i = 0; i < numList.length; i++) {
            if (numList[i] == target) {
                index = i;
                break;
            }
        }

        System.out.println(index);
    }
}