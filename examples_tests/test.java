public class test {
    public static void main(String[] args) {
        int[] data = {3, 14, 5, 8, 10};
        int answer = indexOf(22, data);
        System.out.println(answer);
    }    
    
    static int indexOf(int x, int[] data) {
    // TODO: first index or -1
    int i = 0;
    int indexReturn = 0;
    for (i = 0; i < data.length; i++) {
        if (data[i] == x) {
            return i;
        }
    }
    return -1;
    }
}
