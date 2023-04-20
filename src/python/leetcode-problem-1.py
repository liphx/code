from typing import List

class Solution:
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        data = dict()
        for i, v in enumerate(nums):
            if target - v in data:
                return [data[target - v], i]
            data[v] = i

print(Solution().twoSum([2, 7, 11, 15], 9))
