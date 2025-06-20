#include <bits/stdc++.h>
using namespace std;

int main () {
ios::sync_with_stdio(false);
cin.tie(nullptr);

// Taking inputs
int t; 
cin >> t;
while (t--) {
	int n;
	cin >> n;
      vector<int> p(n);
      for (int i = 0; i < n; ++i) {
          cin >> p[i];
      }

      // main fnc
      if(is_sorted(p.begin(), p.end())) {cout << "yes"; return 0;}
      int first = p[0];
      int min = p[0];   
      for (int i=0; i<n; i++) {
          if (p[i] < min) {min = p[i];}
      }
      if (first == min) {cout << "YES";}
      else {cout << "NO";}
return 0;	
}
}