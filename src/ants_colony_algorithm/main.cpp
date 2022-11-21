#include "iostream"
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

#define M 8//蚂蚁数
#define P 0.2//信息素挥发率
#define MAX 70
int BRoute[MAX];//最优解
int BValue=0;//最优解的总价值
int Bweight;//最优解的总重量
int max_circle=10;//外循环最大次数
int antRoute[9][MAX];
int antValue[9];
int main(){
    int n = 5;//物品个数
    int w_limit = 10;//背包重量限制
    int value[6] = {0,2 ,2 ,6 ,5 ,4 };//各物品价值
    int weight[6] = {0,6,3 ,5 ,4 ,6 };//各物品重量
    float inf[MAX][MAX];//信息素矩阵
    int i,j;

    //************************* 信息素矩阵初始化 ********************//

    for(i=0;i<n;i++)
        inf[i][0]=0;
    for(i=0;i<n;i++){
        for(j=1;j<n;j++){
            inf[i][j]=1;
        }
    }

    //************************ 数据输出 ****************************//
    cout<<"count of the goods: "<<n<<endl;
    cout<<"the limit weight of bag: "<<w_limit<<endl;

    cout<<"the weight of every good: "<<endl;
    for(i=1;i<=n;i++)
        cout<< weight[i]<<" ";
    cout<<endl;
    cout<<"the value of every good: "<<endl;
    for(i=1;i<=n;i++)
        cout<< value[i]<<" ";
    cout<<endl<<endl;
    srand((int)time(0)); //初始化随机种子
    bool mark[MAX];
    int no_modify=0;
    for(int k=1;k<=max_circle && no_modify<100 ;k++){//外循环
        for(i=1;i<=M;i++){
            antRoute[i][0]=0;
            int Cur_n=0; //已选取物品个数
            int Cur_w=0; //已选取物品总重量
            int Cur_v=0; //已选取物品总价值
            int Cur_ps=0; //记录当前选取物品的标号
            for(j=1;j<=n;j++)
                mark[j]=false; //作为物品是否选取的标志
            bool finish=false;
            int ok[MAX];
            while(finish==false){
                if(Cur_n==n){
                    antRoute[i][++Cur_n]=0;
                    antValue[i]=Cur_v;
                    finish=true;

                }
                else{
                    int ok_n=0;
                    for(j=1;j<=n;j++){
                        if(mark[j]==false &&(Cur_w+weight[j])<=w_limit){
                            ok[ok_n++]=j; //该数组用于存储满足条件的物品的标号
                        }
                    }
                    if(ok_n==0){ //无满足条件的物品
                        antRoute[i][++Cur_n]=0;
                        antValue[i]=Cur_v;
                        finish=true;
                    }
                    else{
                        //有满足条件的物品：按信息素来进行随机选取
                        float total=0;
                        float rate[MAX];

                        for(j=0;j<ok_n;j++){
                            float total=total+inf[Cur_ps][ok[j]];
                        }
                        for(j=0;j<ok_n;j++)
                            rate[j]=(inf[Cur_ps][ok[j]]/total);

                        bool choose=false;
                        while(choose==false){
                            double r=(double)(rand() % 1001) * 0.001f;
                            int u=(int)(rand()%ok_n);
                            if(rate[u]>r){
                                antRoute[i][++Cur_n]=ok[u];
                                Cur_ps=ok[u];
                                Cur_w+=weight[ok[u]];
                                Cur_v+=value[ok[u]];
                                mark[ok[u]]=true;
                                choose=true;
                                break;
                            }
                        }




                    }//else

                }//else
            }//while

        }//for:内循环

        //与当前最优解比较
        int temp=0;
        for(i=1;i<=M;i++){
            if(antValue[i]>BValue){
                BValue=antValue[i];
                temp=i;
            }
        }
        if(temp==0)
            no_modify++; //记录连续不改变的次数，达到一定值时停止外循环
        if(temp!=0){
            no_modify=0;
            for(int s=0;s<MAX;s++){
                BRoute[s]=antRoute[temp][s];
            }

        }


        //************************ 信息素更新 ****************************//

        //挥发
        for(i=0;i<=n;i++)
            for(j=0;j<=n;j++){
                inf[i][j]*=(1-P);
            }
        //最优解再进行信息素的增强
        inf[BRoute[0]][BRoute[1]]+=P/n;
        for(int s=1; s<MAX && BRoute[s]!=0;s++){
            inf[BRoute[s]][BRoute[s+1]]+=P/n;
        }

    }//外循环
//    cout<<"外循环次数为："<<k<<endl;
    cout<<"the best case: ";
    for( int k=1;k<MAX && BRoute[k]!=0;k++)
        cout<<BRoute[k]<<" " ;
    cout<<endl<<"the total value: "<<BValue<<endl;
    Bweight=0;
    for(int s=1;s<MAX && BRoute[s]!=0;s++)
        Bweight=Bweight+weight[BRoute[s]];

    cout<<"the total weight: "<<Bweight<<endl;

    return 0;

}