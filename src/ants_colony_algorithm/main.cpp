#include "iostream"
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

#define M 8//������
#define P 0.2//��Ϣ�ػӷ���
#define MAX 70
int BRoute[MAX];//���Ž�
int BValue=0;//���Ž���ܼ�ֵ
int Bweight;//���Ž��������
int max_circle=10;//��ѭ��������
int antRoute[9][MAX];
int antValue[9];
int main(){
    int n = 5;//��Ʒ����
    int w_limit = 10;//������������
    int value[6] = {0,2 ,2 ,6 ,5 ,4 };//����Ʒ��ֵ
    int weight[6] = {0,6,3 ,5 ,4 ,6 };//����Ʒ����
    float inf[MAX][MAX];//��Ϣ�ؾ���
    int i,j;

    //************************* ��Ϣ�ؾ����ʼ�� ********************//

    for(i=0;i<n;i++)
        inf[i][0]=0;
    for(i=0;i<n;i++){
        for(j=1;j<n;j++){
            inf[i][j]=1;
        }
    }

    //************************ ������� ****************************//
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
    srand((int)time(0)); //��ʼ���������
    bool mark[MAX];
    int no_modify=0;
    for(int k=1;k<=max_circle && no_modify<100 ;k++){//��ѭ��
        for(i=1;i<=M;i++){
            antRoute[i][0]=0;
            int Cur_n=0; //��ѡȡ��Ʒ����
            int Cur_w=0; //��ѡȡ��Ʒ������
            int Cur_v=0; //��ѡȡ��Ʒ�ܼ�ֵ
            int Cur_ps=0; //��¼��ǰѡȡ��Ʒ�ı��
            for(j=1;j<=n;j++)
                mark[j]=false; //��Ϊ��Ʒ�Ƿ�ѡȡ�ı�־
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
                            ok[ok_n++]=j; //���������ڴ洢������������Ʒ�ı��
                        }
                    }
                    if(ok_n==0){ //��������������Ʒ
                        antRoute[i][++Cur_n]=0;
                        antValue[i]=Cur_v;
                        finish=true;
                    }
                    else{
                        //��������������Ʒ������Ϣ�����������ѡȡ
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

        }//for:��ѭ��

        //�뵱ǰ���Ž�Ƚ�
        int temp=0;
        for(i=1;i<=M;i++){
            if(antValue[i]>BValue){
                BValue=antValue[i];
                temp=i;
            }
        }
        if(temp==0)
            no_modify++; //��¼�������ı�Ĵ������ﵽһ��ֵʱֹͣ��ѭ��
        if(temp!=0){
            no_modify=0;
            for(int s=0;s<MAX;s++){
                BRoute[s]=antRoute[temp][s];
            }

        }


        //************************ ��Ϣ�ظ��� ****************************//

        //�ӷ�
        for(i=0;i<=n;i++)
            for(j=0;j<=n;j++){
                inf[i][j]*=(1-P);
            }
        //���Ž��ٽ�����Ϣ�ص���ǿ
        inf[BRoute[0]][BRoute[1]]+=P/n;
        for(int s=1; s<MAX && BRoute[s]!=0;s++){
            inf[BRoute[s]][BRoute[s+1]]+=P/n;
        }

    }//��ѭ��
//    cout<<"��ѭ������Ϊ��"<<k<<endl;
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