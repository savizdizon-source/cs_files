public class twoSum {
    public static void main(String[] args) {
        int[] array = {15, 4, 18, 8, 19, 22, 24, 59, 59, 20, 18, 12, 36, 42, 9};
        int[] answer = twoSumBruteForce(array, 24);

        for (int num : answer) {
            System.out.println(num);
        }
    }

    public static int[] twoSumBruteForce(int[] array, int target) {
        int i = 0;
        int j = 0;
        int[] answer = {-1, -1};
        for (i = 0; i < array.length; i++) {
            for (j = 0; j < array.length; j++) {
                if ((i + j) == target) {
                    answer[0] = i;
                    answer[1] = j;
                    return answer;
                }
            }
        }

        return answer;
    }
}
