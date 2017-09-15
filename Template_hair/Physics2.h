#include "Physics1.h"									

class Spring											//������
													
{
public:
	Mass* mass1;										//�ʵ�1
	Mass* mass2;										//�ʵ�2

	float springConstant;								//����ϵ��
	float springLength;									//���ɳ���
	float frictionConstant;								//Ħ��ϵ��

	Spring(Mass* mass1, Mass* mass2, 
		float springConstant, float springLength, float frictionConstant)			//���캯��
	{
		this->springConstant = springConstant;									
		this->springLength = springLength;										
		this->frictionConstant = frictionConstant;								

		this->mass1 = mass1;													
		this->mass2 = mass2;													
	}

	void solve()																	//����������������
	{
		Vector3D springVector = mass1->pos - mass2->pos;							
		
		float r = springVector.length();											//������������֮��ľ���

		Vector3D force;																
		
		if (r != 0)																	//������
			force += (springVector / r) * (r - springLength) * (-springConstant);	

		force += -(mass1->vel - mass2->vel) * frictionConstant;						//����Ħ����
																				

		mass1->applyForce(force);													//������1ʩ����
		mass2->applyForce(-force);													//������2ʩ����
	}

};

// ������
class RopeSimulation : public Simulation				
{
public:
	Spring** springs;									//������ṹ�������ָ��

	Vector3D gravitation;								//��������

	Vector3D ropeConnectionPos;							//���������ӵ�
	
	Vector3D ropeConnectionVel;							//���ӵ���ٶȣ�����ʹ������ƶ�����

	float groundRepulsionConstant;						//����ķ�������
	
	float groundFrictionConstant;						//�����Ħ��ϵ��
	
	float groundAbsorptionConstant;						//����Ļ�����
	
	float groundHeight;									//����߶�

	float airFrictionConstant;							//������Ħ��ϵ��

	RopeSimulation(										//���캯��
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

		for (int a = 0; a < numOfMasses; ++a)			//�����ʵ�λ��
		{
			masses[a]->pos.x = a * springLength;		
			masses[a]->pos.y = 0;						
			masses[a]->pos.z = 0;						
		}

		springs = new Spring*[numOfMasses - 1];			
		
		for (int a = 0; a < numOfMasses - 1; ++a)			//���������ʵ�֮���ģ�ⵯ��
		{
			springs[a] = new Spring(masses[a], masses[a + 1], 
				springConstant, springLength, springFrictionConstant);
		}
	}

	void release()										//�ͷ����е���Դ
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

	void solve()										//����ʩ�Ӹ������ʵ����
	{
		for (int a = 0; a < numOfMasses - 1; ++a)		//����ʩ�Ӹ������������
		{
			springs[a]->solve();						
		}

		for (int a = 0; a < numOfMasses; ++a)				//������������ܵ�����������
		{
			masses[a]->applyForce(gravitation * masses[a]->m);				//��������
			
			masses[a]->applyForce(-masses[a]->vel * airFrictionConstant);	//������Ħ����

			if (masses[a]->pos.y < groundHeight)		//���������ʵ������
			{
				Vector3D v;							

				v = masses[a]->vel;						//�����ٶ�
				v.y = 0;								//y������ٶ�Ϊ0

				//���������ʵ����
				masses[a]->applyForce(-v * groundFrictionConstant);		

				v = masses[a]->vel;						
				v.x = 0;								
				v.z = 0;								
				
				//�������Ļ�����
				if (v.y < 0)							
					masses[a]->applyForce(-v * groundAbsorptionConstant);		
				
				//�������ķ�������
				Vector3D force = Vector3D(0, groundRepulsionConstant, 0) * (groundHeight - masses[a]->pos.y);

				masses[a]->applyForce(force);			//ʩ�ӵ�����ʵ����
			}
				
		}


	}

	void simulate(float dt)								//ģ��һ��
													
	{
		Simulation::simulate(dt);						//���û����ģ�⺯��

		ropeConnectionPos += ropeConnectionVel * dt;	//�������ӵ����ӵ�

		if (ropeConnectionPos.y < groundHeight)			
		{
			ropeConnectionPos.y = groundHeight;
			ropeConnectionVel.y = 0;
		}

		masses[0]->pos = ropeConnectionPos;				//�������ӵ����ӵ���ٶ�
		masses[0]->vel = ropeConnectionVel;				
	}

	void setRopeConnectionVel(Vector3D ropeConnectionVel)	
	{
		this->ropeConnectionVel = ropeConnectionVel;
	}

};