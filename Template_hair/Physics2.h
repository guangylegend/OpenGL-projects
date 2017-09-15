#include "Physics1.h"									

class Spring											//弹簧类
													
{
public:
	Mass* mass1;										//质点1
	Mass* mass2;										//质点2

	float springConstant;								//弹性系数
	float springLength;									//弹簧长度
	float frictionConstant;								//摩擦系数

	Spring(Mass* mass1, Mass* mass2, 
		float springConstant, float springLength, float frictionConstant)			//构造函数
	{
		this->springConstant = springConstant;									
		this->springLength = springLength;										
		this->frictionConstant = frictionConstant;								

		this->mass1 = mass1;													
		this->mass2 = mass2;													
	}

	void solve()																	//计算各个物体的受力
	{
		Vector3D springVector = mass1->pos - mass2->pos;							
		
		float r = springVector.length();											//计算两个物体之间的距离

		Vector3D force;																
		
		if (r != 0)																	//计算力
			force += (springVector / r) * (r - springLength) * (-springConstant);	

		force += -(mass1->vel - mass2->vel) * frictionConstant;						//加上摩擦力
																				

		mass1->applyForce(force);													//给物体1施加力
		mass2->applyForce(-force);													//给物体2施加力
	}

};

// 绳索类
class RopeSimulation : public Simulation				
{
public:
	Spring** springs;									//弹簧类结构的数组的指针

	Vector3D gravitation;								//万有引力

	Vector3D ropeConnectionPos;							//绳索的连接点
	
	Vector3D ropeConnectionVel;							//连接点的速度，我们使用这个移动绳子

	float groundRepulsionConstant;						//地面的反作用力
	
	float groundFrictionConstant;						//地面的摩擦系数
	
	float groundAbsorptionConstant;						//地面的缓冲力
	
	float groundHeight;									//地面高度

	float airFrictionConstant;							//空气的摩擦系数

	RopeSimulation(										//构造函数
		int numOfMasses,								
		float m,										
		float springConstant,							
		float springLength,								
		float springFrictionConstant,					
		Vector3D gravitation,							
		float airFrictionConstant,						
		float groundRepulsionConstant,					
		float groundFrictionConstant,					
		float groundAbsorptionConstant,					
		float groundHeight								
		) : Simulation(numOfMasses, m)					
	{
		this->gravitation = gravitation;
		
		this->airFrictionConstant = airFrictionConstant;

		this->groundFrictionConstant = groundFrictionConstant;
		this->groundRepulsionConstant = groundRepulsionConstant;
		this->groundAbsorptionConstant = groundAbsorptionConstant;
		this->groundHeight = groundHeight;

		for (int a = 0; a < numOfMasses; ++a)			//设置质点位置
		{
			masses[a]->pos.x = a * springLength;		
			masses[a]->pos.y = 0;						
			masses[a]->pos.z = 0;						
		}

		springs = new Spring*[numOfMasses - 1];			
		
		for (int a = 0; a < numOfMasses - 1; ++a)			//创建各个质点之间的模拟弹簧
		{
			springs[a] = new Spring(masses[a], masses[a + 1], 
				springConstant, springLength, springFrictionConstant);
		}
	}

	void release()										//释放所有的资源
	{
		Simulation::release();							

		for (int a = 0; a < numOfMasses - 1; ++a)		
		{
			delete(springs[a]);
			springs[a] = NULL;
		}
		
		delete(springs);
		springs = NULL;
	}

	void solve()										//计算施加给各个质点的力
	{
		for (int a = 0; a < numOfMasses - 1; ++a)		//弹簧施加给各个物体的力
		{
			springs[a]->solve();						
		}

		for (int a = 0; a < numOfMasses; ++a)				//计算各个物体受到的其它的力
		{
			masses[a]->applyForce(gravitation * masses[a]->m);				//万有引力
			
			masses[a]->applyForce(-masses[a]->vel * airFrictionConstant);	//空气的摩擦力

			if (masses[a]->pos.y < groundHeight)		//计算地面对质点的作用
			{
				Vector3D v;							

				v = masses[a]->vel;						//返回速度
				v.y = 0;								//y方向的速度为0

				//计算地面给质点的力
				masses[a]->applyForce(-v * groundFrictionConstant);		

				v = masses[a]->vel;						
				v.x = 0;								
				v.z = 0;								
				
				//计算地面的缓冲力
				if (v.y < 0)							
					masses[a]->applyForce(-v * groundAbsorptionConstant);		
				
				//计算地面的反作用力
				Vector3D force = Vector3D(0, groundRepulsionConstant, 0) * (groundHeight - masses[a]->pos.y);

				masses[a]->applyForce(force);			//施加地面对质点的力
			}
				
		}


	}

	void simulate(float dt)								//模拟一次
													
	{
		Simulation::simulate(dt);						//调用基类的模拟函数

		ropeConnectionPos += ropeConnectionVel * dt;	//计算绳子的连接点

		if (ropeConnectionPos.y < groundHeight)			
		{
			ropeConnectionPos.y = groundHeight;
			ropeConnectionVel.y = 0;
		}

		masses[0]->pos = ropeConnectionPos;				//更新绳子的连接点和速度
		masses[0]->vel = ropeConnectionVel;				
	}

	void setRopeConnectionVel(Vector3D ropeConnectionVel)	
	{
		this->ropeConnectionVel = ropeConnectionVel;
	}

};