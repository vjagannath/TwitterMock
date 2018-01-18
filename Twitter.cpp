#include <iostream>
#include <vector>
#include <set>
#include <map>
using namespace std;

class Twitter {
private:
	map<int, set<int>> users;
	map<int, set<int>>::iterator usersIt;

	map<int, vector<int>> userTweets;
	map<int, vector<int>>::iterator userTweetsIt;
    
    // Helper function to add tweets to the beginning of the tweetlist of the followee users
    void addTweetsToFolloweeList(int userId, int tweetId){
		for (pair<int, set<int>> item : users){
			set<int>::iterator tempIt = item.second.find(userId);
			if (tempIt != (item.second).end()){
				// add tweet to the followee users tweet list
				map<int, vector<int>>::iterator followeeIt = userTweets.find(item.first);
				followeeIt->second.insert(followeeIt->second.begin(), tweetId);
			}
		}
	}

public:
	/** Initialize your data structure here. */
	Twitter() {
	}

	/** Compose a new tweet. */
	void postTweet(int userId, int tweetId) {
		userTweetsIt = userTweets.find(userId);
    
    // check if the user has already posted any tweets
    // Get the tweet list and add the new tweet to the beginning of the list
		if (userTweetsIt != userTweets.end()){
			(userTweetsIt->second).insert(userTweetsIt->second.begin(), tweetId);
			addTweetsToFolloweeList(userId, tweetId);
		}
    // create a new tweet list and add the new tweet to the users tweet list
		else{
			vector<int> newVector{ tweetId };
			userTweets[userId] = newVector;
			addTweetsToFolloweeList(userId, tweetId);
		}
	}

	/** Retrieve the 10 most recent tweet ids in the user's news feed. Each item in the news feed must be posted by users who the user followed or by the user herself. Tweets must be ordered from most recent to least recent. */
	vector<int> getNewsFeed(int userId) {
		vector<int> newsFeeds;
		map<int, vector<int>>::iterator tweets = userTweets.find(userId);
		int maxCount = 10;
    
    // Fetch the latest news feeds for users (upto 10)
		for (int feed : tweets->second){
			if (maxCount){
				newsFeeds.push_back(feed);
			}
			maxCount--;
		}
		return newsFeeds;
	}

	/** Follower follows a followee. If the operation is invalid, it should be a no-op. */
	void follow(int followerId, int followeeId) {
    // Get users followee's follower list and add the new follower
		usersIt = users.find(followerId);
		if (usersIt != users.end()){
			(usersIt->second).insert(followeeId);
		}
		else{
			set<int> newSet{ followeeId };
			users[followerId] = newSet;
		}
	}

	/** Follower unfollows a followee. If the operation is invalid, it should be a no-op. */
	void unfollow(int followerId, int followeeId) {
		usersIt = users.find(followerId);
		if (usersIt != users.end()){
			set<int>::iterator setIt;
			setIt = usersIt->second.find(followeeId);
			(usersIt->second).erase(setIt);

			// delete tweets from the followee in the followers tweet list
			map<int, vector<int>>::iterator followeeIt = userTweets.find(followeeId);
			for (int followeeValue : followeeIt->second){
				map<int, vector<int>>::iterator followerIt = userTweets.find(followerId);
				vector<int>::iterator vecIt;
				bool exists = false;
				for (vecIt = followerIt->second.begin(); vecIt != followerIt->second.end(); vecIt++){
					if (*vecIt == followeeValue){
						// don't remove here because within a loop if we remove we might skip few value checks
						exists = true;
						break;
					}
				}
				if (exists){
					followerIt->second.erase(vecIt);
				}
			}				
		}
	}
};

/**
 * Your Twitter object will be instantiated and called as such:
 * Twitter obj = new Twitter();
 * obj.postTweet(userId,tweetId);
 * vector<int> param_2 = obj.getNewsFeed(userId);
 * obj.follow(followerId,followeeId);
 * obj.unfollow(followerId,followeeId);
 */
