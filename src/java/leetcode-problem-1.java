import java.util.Arrays;
import java.util.HashMap;

class Solution {
    public int[] twoSum(int[] nums, int target) {
        HashMap<Integer, Integer> hashtable = new HashMap<Integer, Integer>();
        for (int i = 0; i < nums.length; i++) {
            if (hashtable.containsKey(target - nums[i])) {
                return new int[] {hashtable.get(target - nums[i]), i};
            }
            hashtable.put(nums[i], i);
        }
        return null;
    }

    public static void main(String args[]) {
        Solution s = new Solution();
        System.out.println(Arrays.toString(s.twoSum(new int[] {2, 7, 9, 11}, 9)));
        System.out.println(Arrays.toString(s.twoSum(new int[] {3, 2, 4}, 6)));
    }
}
