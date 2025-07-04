#include <bits/stdc++.h>
#define ll long long int
#define all(x) (x).begin(), (x).end()
using namespace std;

void solve(){
	string x, y;
	cin >> x >> y;
	
	reverse(all(x));
	reverse(all(y));
	
	ll mv = 0;
	int n = y.size();
	for(int i = 0 ; i < n ; i++){
		if(y[i]=='1'){
			//~ debug() << dbg();
			int nx = x.size();
			
			for(int j = i ; j < nx ; j++){
				if(x[j]=='1'){
					mv = j-i;
					
					break ;
				}
			}
			
			break ;
		}
	}
	
	cout << mv << endl;
}
 
int main()
{
	int t;
	cin >> t;
	while(t--){
		
		solve();
	}
 
	
    return 0;
}