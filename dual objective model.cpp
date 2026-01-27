#include <stdio.h>
#include <time.h>
#include <ilcplex/ilocplex.h>
#include <ilconcert/iloenv.h>
#define M 1e8
#include <math.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <functional>
#include <stdlib.h>
#include <string.h>

using namespace std;

ILOSTLBEGIN

typedef IloArray<IloIntVarArray> IntVarMatrix;
typedef IloArray<IloNumVarArray> NumVarMatrix;
typedef IloArray<IloBoolVarArray> BoolVarMatrix;
typedef IloArray<IloArray<IloBoolVarArray>> BoolVarArray3;
typedef IloArray<IloArray<IloIntVarArray>> IloIntVarArray3;
typedef IloArray<IloArray<IloNumVarArray>> IloNumVarArray3;


int nbSupplier;
int nbProject;
int nbSuppliermax = 10;
int nbProjectmax = 20;
const IloInt testTimes = 3;
const IloNum delta = 1;
int SUM = 0;
int T;
int nbPeriod = 10;
int nbResource = 3;
int C1 = 10;
int C2 = 100;
int Fk = 1000;
double rss = 0.01;
//double Fk = 1000;
int A[] = { 2, 4, 6 };
int AC[] = { 100, 300, 750 };
int BC[] = { 200, 300, 400 };
double LB;
int solutionN = 20;

static vector<int> shelfLife_vector;
static int deltaValue;

const char* resuDir = "D:\\**\\resuFinal.txt";

ofstream outFile;
ofstream resuFile;
ofstream resuFinal(resuDir, ofstream::out);
ifstream dataFile;

IloInt random_initialFun(int* Pduration, double* Prevenue, double** distance, double** Pdemand, double** Scapacity, double** price, double* Srisk, int* budget, int* Goverfs, double** Budgetsav);

int main() {
	clock_t start, start1, start2, start3, start4, start5, start6,start7, end, end1, end2, end3, end4, end5, end6, end7;
	double comptime5 = 0.0;
	for (nbProject = 15; nbProject <= nbProjectmax; nbProject += 5) {
		for (nbSupplier = 10; nbSupplier <= nbSuppliermax; nbSupplier += 10) {
			double** Pdemand = new double* [nbProject];
			double** Scapacity = new double* [nbSupplier];
			double** distance = new double* [nbProject];
			double** Budgetsav = new double* [nbProject];
			int* Pduration = new int[nbProject];
			double* Prevenue = new double[nbProject];
			int* Goverfs = new int[nbPeriod];
			int* budget = new int[nbPeriod];
			double* Srisk = new double[nbSupplier];
			double** price = new double* [nbSupplier];
			for (int i = 0; i < nbSupplier; i++) { Scapacity[i] = new double[nbResource]; }
			for (int i = 0; i < nbSupplier; i++) { price[i] = new double[nbResource]; }
			for (int i = 0; i < nbProject; i++) { Pdemand[i] = new double[nbResource]; }
			for (int i = 0; i < nbProject; i++) { distance[i] = new double[nbSupplier]; }
			for (int i = 0; i < nbProject; i++) { Budgetsav[i] = new double[nbProject]; }

			double tFlag = 0.0;    double TNode = 0.0;   double tTNode = 0.0;
			resuFinal << "Results of instance with " << nbProject << " projects- " << nbSupplier << " suppliers is: " << endl;
			start7 = clock();
			for (T = 0; T < testTimes; T++) {
				resuFinal << "The results of the " << T + 1 << "/5 instance**************************:" << endl;
				int nbSolution = 0;	 double comptime = 0.0, comptime1 = 0.0, comptime2 = 0.0, comptime4 = 0.0, comptime3 = 0.0;
				double epsilon = 0.0; double C1 = 0.0; double D1 = 0.0;
				start = clock();
				stringstream ss;
				stringstream ss1;
				stringstream ss2;
				ss << "D:\\**\\" << ** << "-" << ** << "-(" << T + 1 << ")data.txt";
				ss1 << "D:\\**\\**\\" << **<< "-" << ** << "-(" << T + 1 << ")rst_c.txt";
				ss2 << "D:\\**\\" << ** << "-" << ** << "-(" << ** << ")rst_s.txt";

				string str = ss.str();
				string str1 = ss1.str();
				string str2 = ss2.str();
				dataFile.open(str.c_str(), ios::in);
				outFile.open(str1.c_str(), ios::out);
				resuFile.open(str2.c_str(), ios::out);

				if (!dataFile) { cerr << "error: unable to open data file:" << endl; return(-1); }
				if (!outFile) { cerr << "error: unable to open output file:" << endl; return(-1); }
				if (!resuFile) { cerr << "error: unable to open resuFile file:" << endl; return(-1); }

				resuFinal.setf(ios::fixed, ios::floatfield)  
				resuFinal.precision(4);
				resuFile.setf(ios::fixed, ios::floatfield);
				resuFile.precision(2);
				outFile.setf(ios::fixed, ios::floatfield);
				outFile.precision(2); 

				random_initialFun(Pduration, Prevenue, distance, Pdemand, Scapacity, price, Srisk, budget, Goverfs, Budgetsav);
				//1.定义变量
				IloEnv env;
				try {
					IloModel model(env);

					BoolVarMatrix Xit(env, nbProject);
					for (IloInt i = 0; i < nbProject; i++) {
						Xit[i] = IloBoolVarArray(env, nbPeriod);
					}
					BoolVarArray3 Same(env, nbProject);
					for (IloInt i = 0; i < nbProject; i++) {
						Same[i] = BoolVarMatrix(env, nbProject);
						for (IloInt z = 0; z < nbProject; z++) {
							Same[i][z] = IloBoolVarArray(env, nbPeriod);
						}
					}
					BoolVarMatrix Yis(env, nbProject);
					for (IloInt i = 0; i < nbProject; i++) {
						Yis[i] = IloBoolVarArray(env, nbSupplier);
					}
					IntVarMatrix YMik(env, nbProject);
					for (IloInt i = 0; i < nbProject; i++) {
						YMik[i] = IloIntVarArray(env, nbResource, 0, M);
					}
					IloArray<IloBoolVarArray> BMik(env, nbProject);
					for (IloInt i = 0; i < nbProject; i++) {
						BMik[i] = IloBoolVarArray(env, nbResource);
					}
					IloArray<IloBoolVarArray> CMik(env, nbProject);
					for (IloInt i = 0; i < nbProject; i++) {
						CMik[i] = IloBoolVarArray(env, nbResource);
					}
					IntVarMatrix YYik(env, nbProject);
					for (IloInt i = 0; i < nbProject; i++) {
						YYik[i] = IloIntVarArray(env, nbResource, 0, M);
					}
					IloArray<IloArray<IloBoolVarArray>> Yisk(env, nbProject);
					for (IloInt i = 0; i < nbProject; i++) {
						Yisk[i] = IloArray<IloBoolVarArray>(env, nbSupplier);
						for (IloInt j = 0; j < nbSupplier; j++) {
							Yisk[i][j] = IloBoolVarArray(env, nbResource);
						}
					}
					IloIntVarArray3 Wisk(env, nbProject);
					for (IloInt i = 0; i < nbProject; i++) {
						Wisk[i] = IntVarMatrix(env, nbSupplier);
						for (IloInt j = 0; j < nbSupplier; j++) {
							Wisk[i][j] = IloIntVarArray(env, nbResource, 0, M);
						}
					}
					IloIntVarArray3 numCar(env, nbProject);
					for (IloInt i = 0; i < nbProject; i++) {
						numCar[i] = IntVarMatrix(env, nbSupplier);
						for (IloInt j = 0; j < nbSupplier; j++) {
							numCar[i][j] = IloIntVarArray(env, nbResource, 0, M);
						}
					}
					IloNumVarArray budgetremain(env, nbPeriod, 0, M);

					//Objective function
					IloExpr profitSum(env);
					IloExpr revenueSum(env);
					IloExpr costPurchaseSum(env);
					IloExpr costOtherSum(env);
					IloExpr costNFtranSum(env);
					IloExpr costFtranSum(env);
					IloExpr costInveSum(env);
					IloExpr costSum(env);
					IloExpr costProcurementSum(env);
					IloExpr riskTotal(env);

					int count = 0;
					for (IloInt i = 0; i < nbProject; i++) {
						for (IloInt t = 0; t < nbPeriod; t++) {
							revenueSum += Prevenue[i] * Xit[i][t];
						}
					}

					for (IloInt i = 0; i < nbProject; i++) {
						for (IloInt j = 0; j < nbSupplier; j++) {
							for (IloInt k = 0; k < nbResource; k++) {
								costPurchaseSum += Wisk[i][j][k] * price[j][k];
							}
						}
					}

					for (IloInt i = 0; i < nbProject; i++) {
						for (IloInt j = 0; j < nbSupplier; j++) {
							for (IloInt k = 0; k < nbResource; k++) {
								costNFtranSum += Wisk[i][j][k] * distance[i][j] * C1;
							}
						}
					}

				
					for (IloInt i = 0; i < nbProject; i++) {
						for (IloInt j = 0; j < nbSupplier; j++) {
							for (IloInt k = 0; k < nbResource; k++) {
								costFtranSum += C2 * numCar[i][j][k];
							}
						}
					}

					for (IloInt i = 0; i < nbProject; i++) {
						for (IloInt j = 0; j < nbSupplier; j++) {
							for (IloInt k = 0; k < nbResource; k++) {
								costInveSum += 0.5 * Wisk[i][j][k] * A[k];
							}
						}
					}

					for (IloInt i = 0; i < nbProject; i++) {
						for (IloInt k = 0; k < nbResource; k++) {
							costOtherSum += YYik[i][k] * (AC[k] + BC[k]);
						}
					}
					//2.目标函数
					//目标函数 1
					//profitSum = revenueSum - costProcurementSum - costInveSum - costOtherSum;
					profitSum = revenueSum - costFtranSum - costNFtranSum - costPurchaseSum - costInveSum - costOtherSum;
					IloRange rngC(env, profitSum);

					//目标函数 2
					
					for (IloInt i = 0; i < nbProject; i++) {
						for (IloInt j = 0; j < nbSupplier; j++) {
							for (IloInt k = 0; k < nbResource; k++) {
								riskTotal += Yis[i][j] * Srisk[j] - rss * YMik[i][k];
							}
						}
					}
					IloRange rngD(env, riskTotal);

					IloObjective obj;
					obj = IloMaximize(env, profitSum);
					model.add(obj);

					//Constraints
					// 3. 核心约束
					// 项目执行约束：每个项目最多执行一次
					for (int i = 0; i < nbProject; i++) {
						IloExpr sum(env);
						for (int t = 0; t < nbPeriod; t++) sum += Xit[i][t];
						model.add(sum <= 1);
						sum.end();
					}

					// 资源需求约束
					for (int i = 0; i < nbProject; i++)
						for (int k = 0; k < nbResource; k++) {
							IloExpr sum(env);
							for (int j = 0; j < nbSupplier; j++) sum += Wisk[i][j][k];
							model.add(sum >= Pdemand[i][k]);
							sum.end();
						}

					// 4. 双目标求解（核心逻辑）
					IloCplex cplex(model);
					cplex.setParam(IloCplex::TiLim, TIME_LIMIT);

					// 第一步：最大化利润
					IloObjective obj1 = IloMaximize(env, profit);
					model.add(obj1);
					cplex.solve();
					double maxProfit = cplex.getObjValue();

					// 第二步：最小化风险（固定利润）
					model.remove(obj1);
					model.add(profit >= maxProfit * 0.95);  // 利润不低于最大值的95%
					IloObjective obj2 = IloMinimize(env, risk);
					model.add(obj2);
					cplex.solve();

					// 输出核心结果
					resuFinal << "实例: " << nbProject << "项目-" << nbSupplier << "供应商: " << T + 1 << endl;
					resuFinal << "最大利润: " << maxProfit << " | 最小风险: " << cplex.getObjValue() << endl;

				}
				catch (IloException& e) {
					cerr << "CPLEX异常: " << e << endl;
				}
				catch (...) {
					cerr << "未知异常" << endl;
				}
				env.end();
				dataFile.close();
			}

			// 释放内存
			for (int i = 0; i < nbSupplier; i++) {
				delete[] Scapacity[i];
				delete[] price[i];
			}
			for (int i = 0; i < nbProject; i++) {
				delete[] Pdemand[i];
				delete[] distance[i];
				delete[] Budgetsav[i];
			}
			delete[] Scapacity; delete[] price; delete[] Pdemand;
			delete[] distance; delete[] Budgetsav; delete[] Pduration;
			delete[] Prevenue; delete[] Goverfs; delete[] budget; delete[] Srisk;
		}
	}

	resuFinal.close();
	return 0;
		