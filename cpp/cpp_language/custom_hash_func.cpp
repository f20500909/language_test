#include <bits/stdc++.h>

using namespace std;

class vec3
{
public:
	vec3();
	vec3(int _x, int _y, int _z){x=_x; y=_y; z=_z;}
	int x,y,z;
};

int main()
{	
	struct hashfunc
	{
		size_t operator()(const vec3 v) const
		{
			return size_t(v.x*14514+v.y*19+v.z);
		}	
	};
	struct eqfunc
	{
		bool operator()(const vec3 v1, vec3 v2) const
		{
			return (v1.x==v2.x && v1.y==v2.y && v1.z==v2.z);
		}	
	};
	
	unordered_set<vec3, hashfunc, eqfunc> hash;
	
	while(1)
	{
		int x,y,z; cin>>x>>y>>z;
		vec3 v(x,y,z);
		if(hash.find(v)==hash.end())
			cout<<"not found, insert it"<<endl, hash.insert(v);
		else 
			cout<<"found !"<<endl;
		cout<<endl;
	}
	
	return 0;
}
