#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAMELENGTH 50
#define NUMBEROFDAYSINWEEK 7
#define NUMBEROFINDIVIDUALS 20
#define MAXMIMUMINDIVIDUALSINGROUP 5
#define NUMBEROFGROUPS 5
#define MAXIMUMNUMBEROFGROUPS 10

typedef struct User_Tag
{
    int UserID;
    char UserName[NAMELENGTH];
    int UserAge;
    int DailyStepGoal;
    int WeeklyStepCount[NUMBEROFDAYSINWEEK];
    int GroupPresence; // User is present in group if GroupPresence = 1 else if GroupPresence = 0 then not present
    struct User_Tag *left;
    struct User_Tag *right;
    int UserBalanceFactor;
}User;

typedef struct Group_Tag
{
    int GroupID;
    char GroupName[NAMELENGTH];
    int GroupWeeklyGoal;
    int GroupBalanceFactor;
    User *group_members[MAXMIMUMINDIVIDUALSINGROUP];
    struct Group_Tag *left;
    struct Group_Tag *right;
}Group;

int CalculateUserTreeHeight(User *user_root) // Calculates and returns user node height
{
    int ret_height = -1;
    if(user_root != NULL)
    {
        if((user_root->left == NULL) && (user_root->right == NULL))
        {
            ret_height = 0;
        }
        else
        {
            int h1,h2;
            h1 = CalculateUserTreeHeight(user_root->left);
            h2 = CalculateUserTreeHeight(user_root->right);
            if(h1 > h2)
            {
                ret_height = 1 + h1;
            }
            else
            {
                ret_height = 1 + h2;
            }
        }
    }
    return ret_height;
}

void AssignNewUserTreeBalanceFactors(User *user_root) // Assigns Balance Factors to a particular user node
{
    if (user_root != NULL)
    {
        user_root->UserBalanceFactor = CalculateUserTreeHeight(user_root->left) - CalculateUserTreeHeight(user_root->right);
    }
}

User *UserTreeRotateRight(User *user_root) // Rotates User Tree Right through Root User node
{
    User *temp_user = user_root;
    if (user_root == NULL || user_root->left == NULL)
    {
        printf("Right Rotation not possibe \n");
    }
    else
    {
        temp_user = user_root->left;
        user_root->left = temp_user->right;
        temp_user->right = user_root;
    }
    return temp_user;
}

User *UserTreeRotateLeft(User *user_root) // Rotates User Tree left through root user node
{
    User *temp_user = user_root;
    if (user_root == NULL || user_root->right == NULL)
    {
        printf("Left Rotation not possibe \n");
    }
    else
    {
        temp_user = user_root->right;
        user_root->right = temp_user->left;
        temp_user->left = user_root;
    }
    return temp_user;
}

User *CheckUserTreeImbalance(User *user_root) // Checks and resolves imbalance at user root node
{
    User *ret_user_root = NULL;
    if (user_root == NULL)
    {
        ret_user_root = user_root;
    }
    else
    {
        if (user_root->UserBalanceFactor == 2)
        {
            if (user_root->left->UserBalanceFactor >= 0)
            {
                user_root = UserTreeRotateRight(user_root);
            }
            else
            {
                user_root->left = UserTreeRotateLeft(user_root->left);
                user_root = UserTreeRotateRight(user_root);
            }
        }
        else if (user_root->UserBalanceFactor == -2)
        {
            if (user_root->right->UserBalanceFactor <= 0)
            {
                    user_root = UserTreeRotateLeft(user_root);
            }
            else
            {
                user_root->right = UserTreeRotateRight(user_root->right);
                user_root = UserTreeRotateLeft(user_root);
            }
        }
    }
    ret_user_root = user_root;
    return ret_user_root;
}

User *AddUser(User *user_root, User *user) // Adds new user node to user tree while simultaneously making sure that the tree still remains AVL tree
{
    User *ret_user_root;
    if (user_root == NULL)
    {
        ret_user_root = user;
        printf("Succeccfully added a user \n");
        return ret_user_root;
    }
    else
    {
        if(user->UserID < user_root->UserID)
        {
            user_root->left = AddUser(user_root->left, user);
        }
        else if(user->UserID > user_root->UserID)
        {
            user_root->right = AddUser(user_root->right, user);
        }    
        else
        {
            printf("Duplicate User IDs \n");
        }

        AssignNewUserTreeBalanceFactors(user_root);
        ret_user_root = CheckUserTreeImbalance(user_root);
        return ret_user_root;
    }
    
}

User *CreateUser(int user_id, char user_name[], int user_age, int daily_step_goal, int weekly_step_count[]) // Creates and returns a new user node pointer
{
    int i;
    User *new_user_ptr = (User *)malloc(sizeof(User));
    if (new_user_ptr == NULL)
    {
        printf("Memory allocation failed \n");
    }
    else
    {
        new_user_ptr->UserID = user_id;
        strcpy(new_user_ptr->UserName,user_name);
        new_user_ptr->UserAge = user_age;
        new_user_ptr->DailyStepGoal = daily_step_goal;
        new_user_ptr->UserBalanceFactor = 0;
        for(i = 0;i < NUMBEROFDAYSINWEEK;i++)
        {
            new_user_ptr->WeeklyStepCount[i] = weekly_step_count[i];
        }
        new_user_ptr->GroupPresence = 0;
        new_user_ptr->left = NULL;
        new_user_ptr->right = NULL;
    }
    return new_user_ptr;
}

User *CreateUserTree() // Creates entire user tree
{
    User *user_root = NULL;
    int i, j, id, age, daily_step_goal, weekly_step_count[NUMBEROFDAYSINWEEK];
    char name[NAMELENGTH];
    FILE *file_ptr = fopen("Assignment1DataSample2.txt", "r");
    if (file_ptr == NULL)
    {
        printf("Failed to open file \n");
    }
    else
    {
        for (i = 0; i < NUMBEROFINDIVIDUALS; i++)
        {
            fscanf(file_ptr, "%d", &id);
            fscanf(file_ptr, "%49s", name);
            fscanf(file_ptr, "%d", &age);
            fscanf(file_ptr, "%d", &daily_step_goal);
            for (j = 0; j < NUMBEROFDAYSINWEEK; j++)
            {
                fscanf(file_ptr, "%d", &weekly_step_count[j]);
            }
            User *user_ptr = CreateUser(id, name, age, daily_step_goal, weekly_step_count);
            user_root = AddUser(user_root, user_ptr);
        }
    }
    fclose(file_ptr);
    return user_root;
}

void DisplayUserTree(User *user_root) // Displays User Tree Data in Inorder traversal
{
    if (user_root != NULL)
    {
        DisplayUserTree(user_root->left);
        printf("User ID: %d \n",user_root->UserID);
        DisplayUserTree(user_root->right);
    }
}

void UpdateUserTreeInOutputFile(User *user_root, FILE *file_ptr) // Updates user tree contents in output file
{
    int i;
    if (user_root != NULL)
    {
        UpdateUserTreeInOutputFile(user_root->left, file_ptr);
        fprintf(file_ptr, "%d \n", user_root->UserID);
        fprintf(file_ptr, "%s \n", user_root->UserName);
        fprintf(file_ptr, "%d \n", user_root->UserAge);
        fprintf(file_ptr, "%d \n", user_root->DailyStepGoal);
        for (i = 0; i < NUMBEROFDAYSINWEEK; i++)
        {
            fprintf(file_ptr, "%d \n", user_root->WeeklyStepCount[i]);
        }
        fprintf(file_ptr, "\n");
        UpdateUserTreeInOutputFile(user_root->right, file_ptr);
    }
}

User * SearchUserID(User *user_root,int user_id) // Searches user based on userID and returns its user pointer if found else returns NULL
{
    int found = 0;
    User *user_ptr = NULL;
    if(user_root != NULL)
    {
        user_ptr = user_root;
        while((user_ptr != NULL) && (found == 0))
        {
            if(user_ptr->UserID < user_id)
            {
                user_ptr = user_ptr->right;
            }
            else if(user_ptr->UserID > user_id)
            {
                user_ptr = user_ptr->left;
            }
            else if(user_ptr->UserID == user_id)
            {
                found = 1;
            }
        }
    }
    return user_ptr;
}

int CalculateGroupTreeHeight(Group *group_root) // Calculates and returns group tree height
{
    int ret_height = -1;
    if(group_root != NULL)
    {
        if((group_root->left == NULL) && (group_root->right == NULL))
        {
            ret_height = 0;
        }
        else
        {
            int h1,h2;
            h1 = CalculateGroupTreeHeight(group_root->left);
            h2 = CalculateGroupTreeHeight(group_root->right);
            if(h1 > h2)
            {
                ret_height = 1 + h1;
            }
            else
            {
                ret_height = 1 + h2;
            }
        }
    }
    return ret_height;
}

void AssignNewGroupTreeBalanceFactors(Group *group_root) // Assigns balance factor to given group root node
{
    if (group_root != NULL)
    {
        group_root->GroupBalanceFactor = CalculateGroupTreeHeight(group_root->left) - CalculateGroupTreeHeight(group_root->right);
    }
}

Group *GroupTreeRotateRight(Group *group_root) // Rotates group tree right through group root node
{
    Group *temp_group = group_root;
    if (group_root == NULL || group_root->left == NULL)
    {
        printf("Right Rotation not possibe \n");
    }
    else
    {
        temp_group = group_root->left;
        group_root->left = temp_group->right;
        temp_group->right = group_root;
    }
    return temp_group;
}

Group *GroupTreeRotateLeft(Group *group_root) // Rotates group tree left through group root node
{
    Group *temp_group = group_root;
    if (group_root == NULL || group_root->right == NULL)
    {
        printf("Left Rotation not possibe \n");
    }
    else
    {
        temp_group = group_root->right;
        group_root->right = temp_group->left;
        temp_group->left = group_root;
    }
    return temp_group;
}

Group *CheckGroupTreeImbalance(Group *group_root) // Checks and resolves imbalance
{
    Group *ret_group_root = NULL;
    if (group_root == NULL)
    {
        ret_group_root = group_root;
    }
    else
    {
        if (group_root->GroupBalanceFactor == 2)
        {
            if (group_root->left->GroupBalanceFactor >= 0)
            {
                group_root = GroupTreeRotateRight(group_root);
            }
            else
            {
                group_root->left = GroupTreeRotateLeft(group_root->left);
                group_root = GroupTreeRotateRight(group_root);
            }
        }
        else if (group_root->GroupBalanceFactor == -2)
        {
            if (group_root->right->GroupBalanceFactor <= 0)
            {
                group_root = GroupTreeRotateLeft(group_root);
            }
            else
            {
                group_root->right = GroupTreeRotateRight(group_root->right);
                group_root = GroupTreeRotateLeft(group_root);
            }
        }
    }
    ret_group_root = group_root;
    return ret_group_root;
}

Group * CreateGroup(int group_id,char group_name[],int weekly_group_goal) // Creates new group and return its group pointer
{
    int i;
    Group *new_group_ptr = (Group *)malloc(sizeof(Group));
    if(new_group_ptr == NULL)
    {
        printf("Memory allocation failed");
    }
    else
    {
        i = 0;
        new_group_ptr->GroupID = group_id;
        strcpy(new_group_ptr->GroupName,group_name);
        new_group_ptr->GroupWeeklyGoal = weekly_group_goal;
        for(i = 0;i < MAXMIMUMINDIVIDUALSINGROUP;i++)
        {
            new_group_ptr->group_members[i] = NULL;
        }
        new_group_ptr->GroupBalanceFactor = 0;
        new_group_ptr->left = NULL;
        new_group_ptr->right = NULL;
        printf("New group created successfully \n");
    }
    return new_group_ptr;
}

Group *InsertUserInsideGroup(int group_id,char group_name[],int weekly_group_goal,User *user_ptr,Group *group_root) // Searches group with given groupID -> If found adds user to it -> Else creates a new group and adds user to it
{
    if(group_root != NULL)
    {
        if(group_root->GroupID == group_id)
        {
            int i = 0,done = 0;
            while(i < MAXMIMUMINDIVIDUALSINGROUP && done == 0)
            {
                if(group_root->group_members[i] == NULL)
                {
                    if(user_ptr->GroupPresence == 0)
                    {
                        group_root->group_members[i] = user_ptr;
                        user_ptr->GroupPresence = 1;
                        done = 1;
                        printf("User added succeccfully to group \n");
                    }
                    else
                    {
                        printf("User is already present in a group \n");
                    }
                }
                i = i + 1;
            }
            if(done == 0)
            {
                printf("Group is FULL \n");
            }
        }
        else if(group_root->left == NULL && group_root->GroupID > group_id)
        {
            Group *new_group_ptr = CreateGroup(group_id,group_name,weekly_group_goal);
            if(user_ptr->GroupPresence == 0)
            {
                new_group_ptr->group_members[0] = user_ptr;
                user_ptr->GroupPresence = 1;
                printf("User added succeccfully to group \n");
            }
            else
            {
                printf("User is already present in a group \n");
            }
            group_root->left = new_group_ptr;
        }
        else if(group_root->left != NULL && group_root->GroupID > group_id)
        {
            group_root->left = InsertUserInsideGroup(group_id,group_name,weekly_group_goal,user_ptr,group_root->left);
        }
        else if(group_root->right == NULL && group_root->GroupID < group_id)
        {
            Group *new_group_ptr = CreateGroup(group_id,group_name,weekly_group_goal);
            if(user_ptr->GroupPresence == 0)
            {
                new_group_ptr->group_members[0] = user_ptr;
                user_ptr->GroupPresence = 1;
                printf("User added succeccfully to group \n");
            }
            else
            {
                printf("User is already present in a group \n");
            }
            group_root->right = new_group_ptr;
        }
        else if(group_root->right != NULL && group_root->GroupID < group_id)
        {
            group_root->right = InsertUserInsideGroup(group_id,group_name,weekly_group_goal,user_ptr,group_root->right);
        }
        AssignNewGroupTreeBalanceFactors(group_root);
        group_root = CheckGroupTreeImbalance(group_root);
        return group_root;
    }
    else
    {
        Group *new_group_ptr = CreateGroup(group_id,group_name,weekly_group_goal);
        if(user_ptr->GroupPresence == 0)
        {
            new_group_ptr->group_members[0] = user_ptr;
            user_ptr->GroupPresence = 1;
            printf("Successfully added user to group \n");
        }
        else
        {
            printf("User is already present in a group \n");
        }
        group_root = new_group_ptr;
        return group_root;
    }
}

Group * CreateGroupTree(Group *group_root,User *user_root) // Creates group tree
{
    int i,j,group_id,weekly_group_goal,group_count,user_id;
    char group_name[NAMELENGTH];
    User *user_ptr;
    FILE *file_ptr = fopen("GroupDataSample.txt","r");
    if(file_ptr != NULL)
    {
        for(i = 0;i < NUMBEROFGROUPS;i++)
        {
            fscanf(file_ptr,"%d",&group_id);
            fscanf(file_ptr,"%s",group_name);
            fscanf(file_ptr,"%d",&weekly_group_goal);
            fscanf(file_ptr,"%d",&group_count);
            for(j = 0;j < group_count;j++)
            {
                fscanf(file_ptr,"%d",&user_id);
                user_ptr = SearchUserID(user_root,user_id); // Searches for user pointer to be passed on to InsertUserInsideGroup function
                if(user_ptr != NULL)
                {
                    group_root = InsertUserInsideGroup(group_id,group_name,weekly_group_goal,user_ptr,group_root); // Creates group list
                }
            }
        }
        fclose(file_ptr);
    }
    return group_root;
}

void DisplayGroupTree(Group *group_root) // Displays group tree data
{
    if (group_root != NULL)
    {
        DisplayGroupTree(group_root->left);
        printf("Group ID: %d \n",group_root->GroupID);
        DisplayGroupTree(group_root->right);
    }
}

void UpdateGroupTreeInOutputFile(Group *group_root, FILE *file_ptr) // Updates group tree in output file
{
    int i;
    if (group_root != NULL && file_ptr != NULL)
    {
        UpdateGroupTreeInOutputFile(group_root->left, file_ptr);
        fprintf(file_ptr, "%d \n", group_root->GroupID);
        fprintf(file_ptr, "%s \n", group_root->GroupName);
        fprintf(file_ptr, "%d \n", group_root->GroupWeeklyGoal);
        for (i = 0; i < MAXMIMUMINDIVIDUALSINGROUP; i++)
        {
            if(group_root->group_members[i] != NULL)
            {
                fprintf(file_ptr, "%d \n", group_root->group_members[i]->UserID);
            }
        }
        fprintf(file_ptr, "\n");
        UpdateGroupTreeInOutputFile(group_root->right, file_ptr);
    }
}

int CheckWhetherDailyGoalCompleted(User *user_ptr) // Returns 1 if a user has not completed his/her daily goal else returns 0
{
    int ret_val = 0,i;
    for(i = 0;i < NUMBEROFDAYSINWEEK;i++)
    {
        if(user_ptr->WeeklyStepCount[i] < user_ptr->DailyStepGoal)
        {
            ret_val = 1;
        }
    }
    return ret_val;
}

void GetDailyGoalCompletedUsers(User *user_root,User **DailyGoalCompletedUsers) // Forms an array of daily goal completed users
{
    int i;
    if(user_root != NULL)
    {
        if(CheckWhetherDailyGoalCompleted(user_root) == 0)
        {
            i = 0;
            while((DailyGoalCompletedUsers[i] != NULL) && (i < NUMBEROFINDIVIDUALS))
            {
                i = i + 1;
            }
            if((i < NUMBEROFINDIVIDUALS) && (DailyGoalCompletedUsers[i] == NULL))
            {
                DailyGoalCompletedUsers[i] = user_root;
            }
        }
        GetDailyGoalCompletedUsers(user_root->left,DailyGoalCompletedUsers);
        GetDailyGoalCompletedUsers(user_root->right,DailyGoalCompletedUsers);
    }
}

int FindMaximumUserSteps(User *user_ptr) // Finds and returns maximum steps covered by a user in a single day of a that week
{
    int max_steps = 0,i;
    if(user_ptr != NULL)
    {
        for(i = 0;i < 7;i++)
        {
            if(max_steps < user_ptr->WeeklyStepCount[i])
            {
                max_steps = user_ptr->WeeklyStepCount[i];
            }
        }
    }
    return max_steps;
}

User ** GetTopThreeUsers(User *user_root) // Forms array of top three users
{
    int i = 0;
    User *DailyGoalCompletedUsers[NUMBEROFINDIVIDUALS];
    for(i = 0;i < NUMBEROFINDIVIDUALS;i++)
    {
        DailyGoalCompletedUsers[i] = NULL;
    }
    User **TopThreeUsers = (User **)malloc(3 * (sizeof(User *)));
    GetDailyGoalCompletedUsers(user_root,&DailyGoalCompletedUsers[0]);
    for(i = 0;i < 3;i++) // Initializing all to NULL
    {
        TopThreeUsers[i] = NULL;
    }
    i = 0;
    while((DailyGoalCompletedUsers[i] != NULL) && (i < NUMBEROFINDIVIDUALS))
    {
        if(TopThreeUsers[0] == NULL || FindMaximumUserSteps(DailyGoalCompletedUsers[i]) > FindMaximumUserSteps(TopThreeUsers[0])) // Updates top user pointer and pushes the earlier top 2 users down by one places
        {
            TopThreeUsers[2] = TopThreeUsers[1];
            TopThreeUsers[1] = TopThreeUsers[0];
            TopThreeUsers[0] = DailyGoalCompletedUsers[i];
        }
        else if(TopThreeUsers[1] == NULL || FindMaximumUserSteps(DailyGoalCompletedUsers[i]) > FindMaximumUserSteps(TopThreeUsers[1])) // Updates top second user pointer and pushes earlier top second user down by 1 place
        {
            TopThreeUsers[2] = TopThreeUsers[1];
            TopThreeUsers[1] = DailyGoalCompletedUsers[i];
        }
        else if(TopThreeUsers[2] == NULL || FindMaximumUserSteps(DailyGoalCompletedUsers[i]) > FindMaximumUserSteps(TopThreeUsers[2])) // Updates top third user pointer
        {
            TopThreeUsers[2] = DailyGoalCompletedUsers[i];
        }
        i = i + 1;
    }
    return TopThreeUsers;
}

Group * SearchGroupID(Group *group_root,int group_id) // Searches groups with groupID and returns group pointer 
{
    if(group_root != NULL)
    {
        if(group_root->GroupID < group_id)
        {
            group_root = SearchGroupID(group_root->right,group_id);
        }
        else if(group_root->GroupID > group_id)
        {
            group_root = SearchGroupID(group_root->left,group_id);
        }
    }
    return group_root;
}

int CalculateGroupWeeklySteps(Group *group_ptr) // Calculates total steps of all users in a group on all 7 days of week
{
    int i = 0, j = 0, total_group_week_steps = 0;
    while (group_ptr->group_members[i] != NULL && i < MAXMIMUMINDIVIDUALSINGROUP)
    {
        for (j = 0; j < NUMBEROFDAYSINWEEK; j++)
        {
            total_group_week_steps += group_ptr->group_members[i]->WeeklyStepCount[j];
        }
        i++;
    }
    return total_group_week_steps;
}

void CheckGroupAchievement(Group *group_root,int group_id) // Checks whether a group has completed its weekly group goal
{
    Group *group_ptr;
    if(group_root == NULL)
    {
        printf("No Groups are present \n");
    }
    else
    {
        group_ptr = SearchGroupID(group_root,group_id);
        if(group_ptr == NULL)
        {
            printf("Group with ID %d is not present \n",group_id);
        }
        else
        {
            int i = 0,total_group_week_steps = 0;
            total_group_week_steps = CalculateGroupWeeklySteps(group_ptr);
            printf("Total steps:%d \n",total_group_week_steps);
            printf("Daily Step Goal:%d \n",group_ptr->GroupWeeklyGoal);
            if(total_group_week_steps > group_ptr->GroupWeeklyGoal)
            {
                printf("Group achievement completed \n");
            }
            else
            {
                printf("Group acheievement not completed \n");
            }
        }
    }
}

void CheckIndividualRewards(User *user_root,int user_id) // Checks whether a user is in the TopThreeUsers and allots points accordingly
{
    int user_points = 0;
    User *user_ptr = SearchUserID(user_root,user_id);
    if(user_ptr != NULL)
    {
        User **TopThreeUsers = GetTopThreeUsers(user_root);
        if(user_ptr->UserID == TopThreeUsers[0]->UserID)
        {
            user_points = 100;
        }
        else if(user_ptr->UserID == TopThreeUsers[1]->UserID)
        {
            user_points = 75;
        }
        else if(user_ptr->UserID == TopThreeUsers[2]->UserID)
        {
            user_points = 50;
        }
        printf("User with ID:%d has earned %d points \n",user_ptr->UserID,user_points);
    }
    else
    {
        printf("Invalid User ID entered \n");
    }
}

void SuggestDailyGoalUpdate(User *user_root,int user_id) // Calculates average of daily step goals of topthreeusers and displays it as it is minimum number of steps required for user to be there in top3 users and then modifies the daily goal target entered by user
{
    int i;
    User **TopThreeUsers = GetTopThreeUsers(user_root);
    int top_three_users_daily_step_goal_average = 0;
    for(i = 0;i < 3;i++)
    {
        if(TopThreeUsers[i] != NULL)
        {
            top_three_users_daily_step_goal_average = top_three_users_daily_step_goal_average + TopThreeUsers[i]->DailyStepGoal;
        }
    }
    top_three_users_daily_step_goal_average = top_three_users_daily_step_goal_average/3;
    User *user_ptr = SearchUserID(user_root,user_id);
    int new_daily_step_goal,flag = 0;
    if((user_ptr == TopThreeUsers[0]) || (user_ptr == TopThreeUsers[1]) || (user_ptr == TopThreeUsers[2]))
    {
        printf("User is already present in top three \n");
        printf("Do you still want to change daily step goal. Enter 0 for yes 1 for no. \n");
        scanf("%d",&flag);
    }
    if((user_ptr != NULL) && (flag == 0))
    {
        printf("Minimum daily steps to get into contention of top three is %d \n",top_three_users_daily_step_goal_average + 1);
        printf("Enter the new daily step goal target %s wants to set: \n",user_ptr->UserName);
        scanf("%d",&new_daily_step_goal);
        user_ptr->DailyStepGoal = new_daily_step_goal;
    }
    else if(user_ptr == NULL)
    {
        printf("Invalid user ID entered \n");
    }
    FILE *file_ptr = fopen("Assignment2UserOutput.txt","w");
    UpdateUserTreeInOutputFile(user_root,file_ptr);
    fclose(file_ptr);
}

Group * GenerateLeaderboard(Group *group_root,Group *leaderboard_root) // Generates group leaderboard based on total steps covered by all it's users in the entire week
{
    if(group_root != NULL)
    {
        int i;
        Group *new_leaderboard_group_ptr = CreateGroup(group_root->GroupID,group_root->GroupName,group_root->GroupWeeklyGoal);
        for(i = 0;i < MAXMIMUMINDIVIDUALSINGROUP;i++)
        {
            new_leaderboard_group_ptr->group_members[i] = group_root->group_members[i];
        }
        if(new_leaderboard_group_ptr != NULL)
        {
            if(leaderboard_root == NULL)
            {
                leaderboard_root = new_leaderboard_group_ptr;
            }
            else
            {
                int done = 0;
                Group *leaderboard_ptr = leaderboard_root;
                while((leaderboard_ptr != NULL) && (done == 0))
                {
                    if(CalculateGroupWeeklySteps(leaderboard_ptr) > CalculateGroupWeeklySteps(new_leaderboard_group_ptr))
                    {
                        if(leaderboard_ptr->left == NULL)
                        {
                            leaderboard_ptr->left = new_leaderboard_group_ptr;
                            done = 1;
                        }
                        else
                        {
                            leaderboard_ptr = leaderboard_ptr->left;
                        }
                    }
                    else if(CalculateGroupWeeklySteps(leaderboard_ptr) < CalculateGroupWeeklySteps(new_leaderboard_group_ptr))
                    {
                        if(leaderboard_ptr->right == NULL)
                        {
                            leaderboard_ptr->right = new_leaderboard_group_ptr;
                            done = 1;
                        }
                        else
                        {
                            leaderboard_ptr = leaderboard_ptr->right;
                        }
                    }
                }
                AssignNewGroupTreeBalanceFactors(leaderboard_root);
                leaderboard_root = CheckGroupTreeImbalance(leaderboard_root);
            }
        }
        leaderboard_root = GenerateLeaderboard(group_root->left,leaderboard_root);
        leaderboard_root = GenerateLeaderboard(group_root->right,leaderboard_root); 
    }
    return leaderboard_root;
}

int GroupRank = 1;

void DisplayLeaderboard(Group *leaderboard_root) // Displays leaderboard
{
    if(leaderboard_root != NULL)
    {
        DisplayLeaderboard(leaderboard_root->right);
        printf("%d.Group ID:%d\tSteps:%d \n",GroupRank,leaderboard_root->GroupID,CalculateGroupWeeklySteps(leaderboard_root));
        GroupRank = GroupRank + 1;
        DisplayLeaderboard(leaderboard_root->left);
    }
}

void UpdateLeaderboardInOutputFile(Group *group_root,FILE *file_ptr)
{
    if(group_root != NULL)
    {
        UpdateLeaderboardInOutputFile(group_root->right,file_ptr);
        fprintf(file_ptr, "%d \n", group_root->GroupID);
        fprintf(file_ptr, "%s \n", group_root->GroupName);
        fprintf(file_ptr, "%d \n", group_root->GroupWeeklyGoal);
        for (int i = 0; i < MAXMIMUMINDIVIDUALSINGROUP; i++)
        {
            if(group_root->group_members[i] != NULL)
            {
                fprintf(file_ptr, "%d \n", group_root->group_members[i]->UserID);
            }
        }
        fprintf(file_ptr, "\n");
        UpdateGroupTreeInOutputFile(group_root->left,file_ptr);
    }
}

int Rank = 0;

void DisplayGroupRangeInformation(Group *group_root,Group *group_ptr_1,Group *group_ptr_2) // Inputs 2 groupIDs and prints all groups informations whose IDs are between these 2 inputted IDs
{
    if(group_root != NULL)
    {
        DisplayGroupRangeInformation(group_root->right,group_ptr_1,group_ptr_2);
        Rank = Rank + 1;
        if((group_root->GroupID > group_ptr_1->GroupID) && (group_root->GroupID < group_ptr_2->GroupID))
        {
            printf("Rank: %d\tGroup ID:%d\t Group Name:%s\tGroup Weekly Goal:%d\n",Rank,group_root->GroupID,group_root->GroupName,group_root->GroupWeeklyGoal);
        }
        DisplayGroupRangeInformation(group_root->left,group_ptr_1,group_ptr_2);
    }
}

Group * GetMinimumGroupID(Group *group) // Returns immediate smallest node
{
    Group *group_ptr = group;
 
    // loop down to find the leftmost leaf
    while (group_ptr->left != NULL)
    {
        group_ptr = group_ptr->left;
    } 
    return group_ptr; 
}

Group * DeleteGroup(Group *group_root,int group_id) // Deletes group from group tree
{
    if(group_root == NULL)
    {
        return group_root;
    }
    if(group_id < group_root->GroupID)
    {
        group_root->left = DeleteGroup(group_root->left,group_id);
    }
    else if(group_id > group_root->GroupID)
    {
        group_root->right = DeleteGroup(group_root->right,group_id);
    }
    else 
    {
        if(group_root->left == NULL || group_root->right == NULL)
        {
            Group *temp_group;
            if(group_root->left != NULL)
            {
                temp_group = group_root->left;
            }
            else
            {
                temp_group = group_root->right;
            }
            if(temp_group == NULL) // No child case
            {
                temp_group = group_root;
                group_root = NULL;
            }
            else
            {
                *group_root = *temp_group; // Copies the content of non-empty child
            }
            free(temp_group);
        }
        else
        {
            Group *temp_group = GetMinimumGroupID(group_root->right);
            group_root->GroupID = temp_group->GroupID;
            group_root->right = DeleteGroup(group_root->right,temp_group->GroupID);
        }
    }
    if(group_root == NULL) //Only 1 user in tree
    {
        return group_root;
    }
    else
    {
        AssignNewGroupTreeBalanceFactors(group_root);
        AssignNewGroupTreeBalanceFactors(group_root->left);
        AssignNewGroupTreeBalanceFactors(group_root->right);
        if(group_root->GroupBalanceFactor > 1 && group_root->left->GroupBalanceFactor >= 0)
        {
            return GroupTreeRotateRight(group_root);
        }
        else if(group_root->GroupBalanceFactor > 1 && group_root->left->GroupBalanceFactor < 0)
        {
            group_root->left = GroupTreeRotateLeft(group_root->left);
            return GroupTreeRotateRight(group_root);
        }
        else if(group_root->GroupBalanceFactor < -1 && group_root->right->GroupBalanceFactor <= 0)
        {
            return GroupTreeRotateLeft(group_root);
        }
        else if(group_root->GroupBalanceFactor < -1 && group_root->right->GroupBalanceFactor > 0)
        {
            group_root->right = GroupTreeRotateRight(group_root->right);
            return GroupTreeRotateLeft(group_root);
        }

    }
    return group_root;
}

User * GetMinimumUserID(User *user) // Returns immediate smallest node
{ 
    User *user_ptr = user;
 
    // loop down to find the leftmost leaf 
    while (user_ptr->left != NULL)
    {
        user_ptr = user_ptr->left;
    } 
    return user_ptr; 
}

User * DeleteUser(User *user_root,int user_id) // Deletes user from user tree
{
    if(user_root == NULL)
    {
        return user_root;
    }
    if(user_id < user_root->UserID)
    {
        user_root->left = DeleteUser(user_root->left,user_id);
    }
    else if(user_id > user_root->UserID)
    {
        user_root->right = DeleteUser(user_root->right,user_id);
    }
    else 
    {
        if(user_root->left == NULL || user_root->right == NULL)
        {
            User *temp_user;
            if(user_root->left != NULL)
            {
                temp_user = user_root->left;
            }
            else
            {
                temp_user = user_root->right;
            }
            if(temp_user == NULL) // No child case
            {
                temp_user = user_root;
                user_root = NULL;
            }
            else
            {
                *user_root = *temp_user; // Copies the content of non-empty child
            }
            free(temp_user);
        }
        else // 2 child case
        {
            User *temp_user = GetMinimumUserID(user_root->right);
            user_root->UserID = temp_user->UserID;
            user_root->right = DeleteUser(user_root->right,temp_user->UserID);
        }
    }
    if(user_root == NULL) //Only 1 user in tree
    {
        return user_root;
    }
    else
    {
        AssignNewUserTreeBalanceFactors(user_root);
        AssignNewUserTreeBalanceFactors(user_root->left);
        AssignNewUserTreeBalanceFactors(user_root->right);
        if(user_root->UserBalanceFactor > 1 && user_root->left->UserBalanceFactor >= 0)
        {
            return UserTreeRotateRight(user_root);
        }
        else if(user_root->UserBalanceFactor > 1 && user_root->left->UserBalanceFactor < 0)
        {
            user_root->left = UserTreeRotateLeft(user_root->left);
            return UserTreeRotateRight(user_root);
        }
        else if(user_root->UserBalanceFactor < -1 && user_root->right->UserBalanceFactor <= 0)
        {
            return UserTreeRotateLeft(user_root);
        }
        else if(user_root->UserBalanceFactor < -1 && user_root->right->UserBalanceFactor > 0)
        {
            user_root->right = UserTreeRotateRight(user_root->right);
            return UserTreeRotateLeft(user_root);
        }

    }
    return user_root;

}

void DeleteUserFromGroup(Group *group_root, int user_id) // Deletes user from particular group where user is present
{
    int i, j;
    if (group_root != NULL)
    {
        for (i = 0; i < MAXMIMUMINDIVIDUALSINGROUP; i++)
        {
            if (group_root->group_members[i] != NULL && group_root->group_members[i]->UserID == user_id)
            {
                for (j = i; j < MAXMIMUMINDIVIDUALSINGROUP - 1; j++)
                {
                    group_root->group_members[j] = group_root->group_members[j + 1];
                }
                group_root->group_members[j] = NULL;
                printf("Successfully Deleted \n");
                
                return; // Exit the function after successful deletion
            }
        }
        // Check if the user is found and deleted before continuing traversal
        if (group_root->left == NULL && group_root->right == NULL)
            return;

        DeleteUserFromGroup(group_root->left, user_id);
        DeleteUserFromGroup(group_root->right, user_id); // Corrected recursive call
    }
}

Group *AddGroup(Group *group_root,Group *new_group_ptr) // Adds given group to group tree and also maintains AVL tree existence
{
    if(new_group_ptr != NULL)
            {
                int done = 0;
                Group *group_ptr = group_root;
                while((group_ptr != NULL) && (done == 0))
                {
                    if(group_ptr->GroupID > new_group_ptr->GroupID)
                    {
                        if(group_ptr->left == NULL)
                        {
                            group_ptr->left = new_group_ptr;
                            done = 1;
                        }
                        else
                        {
                            group_ptr = group_ptr->left;
                        }
                    }
                    else if(group_ptr->GroupID < new_group_ptr->GroupID)
                    {
                        if(group_ptr->right == NULL)
                        {
                            group_ptr->right = new_group_ptr;
                            done = 1;
                        }
                        else
                        {
                            group_ptr = group_ptr->right;
                        }
                    }
                }
                AssignNewGroupTreeBalanceFactors(group_root);
                group_root = CheckGroupTreeImbalance(group_root);
            }
}

int GetTotalStepsCompletedByUser(User *user_ptr)
{
    int total_steps = 0;
    for(int i = 0;i < NUMBEROFDAYSINWEEK;i++)
    {
        total_steps = total_steps + user_ptr->WeeklyStepCount[i];
    }
    return total_steps;
}

int main()
{
    int i;
    User *user_root = CreateUserTree();
    FILE *user_file_ptr = fopen("Assignment2UserOutput2.txt","w");
    UpdateUserTreeInOutputFile(user_root,user_file_ptr);
    fclose(user_file_ptr);
    Group *group_root = NULL;
    group_root = CreateGroupTree(group_root,user_root);
    FILE *group_file_ptr = fopen("Assignment2GroupOutput2.txt","w");
    UpdateGroupTreeInOutputFile(group_root,group_file_ptr);
    fclose(group_file_ptr);
    printf("Done \n");
    int choice_num = 1;
    printf("\t\t\t\t\t\t\t STEP TRACKING APPLICATION \t\t\n");
    while(choice_num != 0)
    {
        printf("Enter which function number you want to execute: \n");
        printf("\n 0. EXIT \n");
        printf("\n 1. Add Person \n");
        printf("\n 2. Create Group \n");
        printf("\n 3. Get Top 3 \n");
        printf("\n 4. Check Group Achievement \n");
        printf("\n 5. Generate Leaderboard \n");
        printf("\n 6. Check Individual Rewards \n");
        printf("\n 7. Delete Individual \n");
        printf("\n 8. Delete Group \n");
        printf("\n 9. Merge Groups \n");
        printf("\n 10. Display Group Range Information \n");
        printf("\n 11. Suggest Individual's Daily Goal Update \n\n");
        scanf("%d",&choice_num);
        switch(choice_num)
        {
            case 0:
            {
                printf("\n\nLEAVING THE APPLICATION \n\n");
                break;
            }
            case 1:
            {
                int user_id,daily_step_goal;
                printf("Enter User ID \n");
                scanf("%d",&user_id);
                printf("Enter Daily step goal \n");
                scanf("%d",&daily_step_goal);
                int weekly_step_count[NUMBEROFDAYSINWEEK] = {10000,2000,30000,4000,50000,60000,70001};
                User *user = CreateUser(user_id,"Rish",19,daily_step_goal,weekly_step_count);
                user_root = AddUser(user_root,user);
                user_file_ptr = fopen("Assignment2UserOutput2.txt","w");
                UpdateUserTreeInOutputFile(user_root,user_file_ptr);
                fclose(user_file_ptr);
                if(user_root == NULL)
                {
                    printf("NULL \n");
                }
                break;
            }
            case 2:
            {
                int user_id,group_id,weekly_group_goal;
                printf("Enter User ID \n");
                scanf("%d",&user_id);
                printf("Enter Group ID \n");
                scanf("%d",&group_id);
                printf("Enter weekly group goal \n");
                scanf("%d",&weekly_group_goal);
                User *user = SearchUserID(user_root,user_id);
                if(user != NULL)
                {
                    group_root = InsertUserInsideGroup(group_id,"Evaluation",weekly_group_goal,user,group_root);
                    group_file_ptr = fopen("Assignment2GroupOutput2.txt","w");
                    UpdateGroupTreeInOutputFile(group_root,group_file_ptr);
                    fclose(group_file_ptr);
                }
                else
                {
                    Group *group_ptr = SearchGroupID(group_root,group_id);
                    if(group_ptr == NULL)
                    {
                        Group *new_group_ptr = CreateGroup(group_id,"Evaluation",weekly_group_goal);
                        group_root = AddGroup(group_root,new_group_ptr);
                        group_file_ptr = fopen("Assignment2GroupOutput2.txt","w");
                        UpdateGroupTreeInOutputFile(group_root,group_file_ptr);
                        fclose(group_file_ptr);
                        printf("Invalid User but empty Group Created \n");
                    }
                    else 
                    {
                        printf("Invalid User Group already present \n");
                    }
                }
                break;
            }
            case 3:
            {
                User **TopThreeUsers = GetTopThreeUsers(user_root);
                printf("Top Three Users are: \n");
                for(i = 0;i < 3;i++)
                {
                    if(TopThreeUsers[i] != NULL)
                    {
                        printf("%d.%s\t\tID:%d\t\tSteps:%d\n",i+1,TopThreeUsers[i]->UserName,TopThreeUsers[i]->UserID,GetTotalStepsCompletedByUser(TopThreeUsers[i]));
                    }
                }
                break;
            }
            case 4:
            {
                int group_id;
                printf("Enter user ID \n");
                scanf("%d",&group_id);
                CheckGroupAchievement(group_root,group_id);
                break;
            }
            case 5:
            {
                Group *leaderboard = GenerateLeaderboard(group_root,NULL);
                printf("Group Leaderboard: \n");
                DisplayLeaderboard(leaderboard);
                group_file_ptr = fopen("Assignment2Leaderboard2.txt","w");
                UpdateLeaderboardInOutputFile(leaderboard,group_file_ptr);
                fclose(group_file_ptr);
                break;
            }
            case 6:
            {
                int user_id;
                printf("Enter UserID \n");
                scanf("%d",&user_id);
                CheckIndividualRewards(user_root,user_id);
                break;
            }
            case 7:
            {
                int delete_user_id;
                printf("Enter UserID of user to be deleted \n");
                scanf("%d",&delete_user_id);
                User *delete_user_ptr = SearchUserID(user_root,delete_user_id);
                if(delete_user_ptr != NULL)
                {
                    DeleteUserFromGroup(group_root,delete_user_id);
                    group_file_ptr = fopen("Assignment2GroupOutput2.txt","w");
                    UpdateGroupTreeInOutputFile(group_root,group_file_ptr);
                    fclose(group_file_ptr);
                    user_root = DeleteUser(user_root,delete_user_id);
                    user_file_ptr = fopen("Assignment2UserOutput2.txt","w");
                    UpdateUserTreeInOutputFile(user_root,user_file_ptr);
                    fclose(user_file_ptr);
                }
                else
                {
                    printf("Invalid UserID entered \n");
                }
                break;
            }
            case 8:
            {
                int i,delete_group_id;
                printf("Enter GroupID of group to be deleted \n");
                scanf("%d",&delete_group_id);
                Group *group_ptr = SearchGroupID(group_root,delete_group_id);
                if(group_ptr != NULL)
                {
                    for(i = 0;i < MAXMIMUMINDIVIDUALSINGROUP;i++)
                    {
                        if(group_ptr->group_members[i] != NULL)
                        {
                            group_ptr->group_members[i]->GroupPresence = 0;
                        }
                        group_ptr->group_members[i] = NULL;
                    }
                    group_root = DeleteGroup(group_root,delete_group_id);
                    DisplayGroupTree(group_root);
                    group_file_ptr = fopen("Assignment2GroupOutput2.txt","w");
                    UpdateGroupTreeInOutputFile(group_root,group_file_ptr);
                    fclose(group_file_ptr);
                }
                else
                {
                    printf("Invalid GroupID entered \n");
                }
                break;
            }
            case 9:
            {
                int group_id_1,group_id_2;
                printf("enter both group id to br merged \n");
                scanf("%d %d",&group_id_1,&group_id_2);
                Group *group_ptr_1 = SearchGroupID(group_root,group_id_1);
                Group *group_ptr_2 = SearchGroupID(group_root,group_id_2);
                if(group_ptr_1 != NULL && group_ptr_2 != NULL)
                {
                    int total_users = 0,i;
                    for(i = 0;i < MAXMIMUMINDIVIDUALSINGROUP;i++)
                    {
                        if(group_ptr_1->group_members[i] != NULL)
                        {
                            total_users = total_users + 1;
                        }
                    }
                    for(i = 0;i < MAXMIMUMINDIVIDUALSINGROUP;i++)
                    {
                        if(group_ptr_2->group_members[i] != NULL)
                        {
                            total_users = total_users + 1;
                        }
                    }
                    if(total_users > 5)
                    {
                        printf("Merging not possible \n");
                    }
                    else
                    {
                        int j = 0;
                        int group_id = ((10 * group_ptr_1->GroupID) + group_ptr_2->GroupID);
                        Group *new_group_ptr = CreateGroup(group_id,"Merged Group",group_ptr_1->GroupWeeklyGoal + group_ptr_2->GroupWeeklyGoal);
                        for(i = 0;i < MAXMIMUMINDIVIDUALSINGROUP;i++)
                        {
                            if(group_ptr_1->group_members[i] != NULL)
                            {
                                new_group_ptr->group_members[j] = group_ptr_1->group_members[i];
                                j = j + 1;
                            }
                        }
                        for(i = 0;i < MAXMIMUMINDIVIDUALSINGROUP;i++)
                        {
                            if(group_ptr_2->group_members[i] != NULL)
                            {
                                new_group_ptr->group_members[j] = group_ptr_2->group_members[i];
                                j = j + 1;
                            }
                        }
                        group_root = DeleteGroup(group_root,group_ptr_1->GroupID);
                        group_root = DeleteGroup(group_root,group_ptr_2->GroupID);
                        for(i = 0;i < MAXMIMUMINDIVIDUALSINGROUP;i++)
                        {
                            new_group_ptr->group_members[i]->GroupPresence = 1;
                        }
                        group_root = AddGroup(group_root,new_group_ptr);
                        DisplayGroupTree(group_root);
                        group_file_ptr = fopen("Assignment2GroupOutput2.txt","w");
                        UpdateGroupTreeInOutputFile(group_root,group_file_ptr);
                        fclose(group_file_ptr);
                        int i;
                        for(i = 0;i < MAXMIMUMINDIVIDUALSINGROUP;i++)
                        {
                            if(new_group_ptr->group_members[i] != NULL)
                            {
                                new_group_ptr->group_members[i]->GroupPresence = 1;
                            }
                        }
                    }
                }
                else
                {
                    printf("Merging not possible \n");
                }
                break;
                
            }
            case 10:
            {
                int group_id_1,group_id_2;
                printf("Enter two ids to display group range info\n");
                scanf("%d %d",&group_id_1,&group_id_2);
                Group *leaderboard_root =  NULL;
                leaderboard_root = GenerateLeaderboard(group_root,leaderboard_root);
                Group *group_ptr_1 = SearchGroupID(group_root,group_id_1);
                Group *group_ptr_2 = SearchGroupID(group_root,group_id_2);
                DisplayGroupRangeInformation(leaderboard_root,group_ptr_1,group_ptr_2);
                break;
            }
            case 11:
            {
                int user_id;
                printf("Enter id to suggest daily goal update\n");
                scanf("%d",&user_id);
                SuggestDailyGoalUpdate(user_root,user_id);
                break;
            }
            default:
            {
                printf("\n\n INVALID OPTION \n\n");
                break;
            }
        }
    }
    return 0;
}


