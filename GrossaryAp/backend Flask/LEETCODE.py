# 2 SUM 
"""
# this is what i did using 2 for loops, but its time complexity can be made better. Time complexity bad, space complexity good
class Solution:
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        for i in range(len(nums)):
            for j in range(i+1, len(nums)):
                if nums[i] == target - nums[j]:
                    return(i,j)
                
# this is where the concept of hashmap is being used and it's time complexity is better space complexity is bad.
class Solution:
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        num_to_index = {}  # A dictionary to store numbers and their indices
        
        for i, num in enumerate(nums):
            complement = target - num
            
            # Check if the complement (the number needed to reach the target) is in the dictionary
            if complement in num_to_index:
                return [num_to_index[complement], i]
            
            # If not found, store the current number and its index in the dictionary
            num_to_index[num] = i
        
        # If no solution is found, return an empty list or raise an exception as needed
        return []"""

