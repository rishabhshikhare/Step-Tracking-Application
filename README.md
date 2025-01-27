# Step-Tracking-Application
This project is a step-tracking application designed using tree data structures to manage individuals and groups. The application promotes fitness by encouraging individuals to meet daily step goals, rewarding high achievers, and maintaining leaderboards for both individuals and groups. The system leverages efficient file handling for data input and storage and provides multiple functionalities to enhance user engagement.

Features:

1. Individual Management
Tree of Individuals:
A binary search tree sorted by unique IDs, where each individual has:
ID (unique identifier)
Name
Age
Daily step goal
Array of weekly step counts (7 days' data)
Add_Person: Add a new individual while maintaining the sorted structure of the tree.

2. Group Management
Tree of Groups:
A binary search tree sorted by Group-ID, where each group has:
Group-ID
Group name
Member IDs (pointers to individuals in the individual tree)
Weekly group goal
Create_group: Create a new group with up to 5 members. Ensure individuals already in a group cannot join another group.

3. Leaderboards and Rewards
Get_top_3: Display the top 3 individuals who have met their daily step goals and achieved the highest number of steps.
Generate_leader_board: Rank groups in descending order of total weekly steps and display the results.

4. Achievement and Goal Tracking
Check_group_achievement: Verify if a given group has achieved its weekly goal.
Check_individual_rewards: Display rewards for individuals in the top 3:
Rank 1: 100 points
Rank 2: 75 points
Rank 3: 50 points

5. Data Modification
Delete_individual: Remove an individual from the individual tree and all associated groups.
Delete_group: Delete a group but retain its individuals, making them available for other groups.
Merge_groups: Merge two groups into a new one with a combined goal. The group with Group_ID_1 becomes the new group, and Group_ID_2 is deleted.

6. Additional Functionalities
Display_group_range_info: Show information about groups and members within a specified range of Group-IDs, including goals and ranks.
Suggest_goal_update: Recommend a new daily goal for an individual to help them consistently rank in the top 3.
Input Data
The application uses a .txt file to load initial data, which includes:
20 individuals with their details (ID, name, age, daily step goal, weekly step count).
5 groups with varying members, some individuals not belonging to any group.
