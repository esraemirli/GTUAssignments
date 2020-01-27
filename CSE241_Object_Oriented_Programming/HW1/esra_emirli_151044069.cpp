#include<iostream>
#include<cstdlib>
#include <cmath>
using namespace std;

//Normal fibonacci function that return last element..
int fib(int size){
  int n1=1,n2=1,n3=1;
  for(int i=0;i<size;i++){
    if(i==0 || i==1)
      n3=1; //last element
    else {
      n3=n1+n2;
      n1=n2;
      n2=n3;
    }
  }
  return n3;
}

int main(){

  int num,size,column,factor;
  cin>>num;
  //Check negativity and limit of int..
  if(num<0)
    exit(-1);
  //Maximum size can be 46 because of int limit..
  else if(num>45)
    exit(-1);
  size=num+1;

  //All rows are saved in values array..
  int values[size];
  //Middle line of pyramid will back up from values array.
  int middle_line[size];
  int middle = round( (double)size / 2 ) + 1;
  // Columns multiplied by a factor of fibonacci numbers. Also factor is increasing like fibonacci..
  for(int i=0;i<size;i++){
    // index for values array..
    int index=0;
    //Keeps columns number..
    column=1;
    for(int j=i+1;j>0;j--){
      //Factor : 1 1 2 3 5 ... is fibonacci series.
      factor=fib(column);
      if(column<3){
        cout<<fib(j)<<" ";
        values[index]=factor*fib(j);
        index++;
      }
      //Other columns multiplied with factor..
      else{
        cout<<factor*fib(j)<<" ";
        values[index]=factor*fib(j);
        index++;
      }
      column++;
    }
    cout<<endl;
    //Back up heart line..
    if(i==middle-1){
      for(int k=0;k<size;k++)
        middle_line[k]=values[k];
    }
  }
  //Last element of fibonacci is biggest number..
  int max=fib(size);
  cout<<"Biggest number: "<<max<<endl;
  //If middle is odd, heart has a number..
  if(middle%2!=0 || num==0)
    cout<<"Heart of the pyramid: "<<middle_line[middle/2]<<endl;
  //If middle is even, heart has two number..
  else
    cout<<"Heart of the pyramid: "<<middle_line[middle/2-1]<<" "<<middle_line[middle/2]<<endl;

  return 0;
}
