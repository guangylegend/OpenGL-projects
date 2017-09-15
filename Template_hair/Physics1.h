#include <math.h>

// ����һ��3Dλ�õ���
class Vector3D
{
public:
	float x;									
	float y;									
	float z;									

	Vector3D()									// ��ʼ�� x = y = z = 0
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3D(float x, float y, float z)			// ��ʼ��
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3D& operator= (Vector3D v)			// ��ֵ
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	Vector3D operator+ (Vector3D v)				// ��
	{
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}

	Vector3D operator- (Vector3D v)				// ��
	{
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}

	Vector3D operator* (float value)			// ��
	{
		return Vector3D(x * value, y * value, z * value);
	}

	Vector3D operator/ (float value)			// ��
	{
		return Vector3D(x / value, y / value, z / value);
	}

	Vector3D& operator+= (Vector3D v)			
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3D& operator-= (Vector3D v)			
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3D& operator*= (float value)			
	{
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	Vector3D& operator/= (float value)			
	{
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}


	Vector3D operator- ()						
	{
		return Vector3D(-x, -y, -z);
	}

	float length()								//���س���
	{
		return sqrtf(x*x + y*y + z*z);
	};			   		

	void unitize()								// ��һ��
	{
		float length = this->length();

		if (length == 0)
			return;

		x /= length;
		y /= length;
		z /= length;
	}

	Vector3D unit()								// ���ص�λ����
	{
		float length = this->length();

		if (length == 0)
			return *this;
		
		return Vector3D(x / length, y / length, z / length);
	}

};

// ��ʾһ������
class Mass
{
public:
	float m;									// ����
	Vector3D pos;								// λ��
	Vector3D vel;								// �ٶ�
	Vector3D force;								// ��

	Mass(float m)								// ���캯��
	{
		this->m = m;
	}

	// ����һ����
	void applyForce(Vector3D force)
	{
		this->force += force;				
	}

	// ��ʼʱ����Ϊ0
	void init()
	{
		force.x = 0;
		force.y = 0;
		force.z = 0;
	}

	//���������ʱ��dt��ģ������Ч��
	void simulate(float dt)
	{
		vel += (force / m) * dt;			
											

		pos += vel * dt;					
											
	}

};

// ģ����
class Simulation
{
public:
	int numOfMasses;								// ����ĸ���
	Mass** masses;									// ָ������ṹ��ָ��
	
	Simulation(int numOfMasses, float m)			// ���캯��
	{
		this->numOfMasses = numOfMasses;
		
		masses = new Mass*[numOfMasses];			

		for (int a = 0; a < numOfMasses; ++a)		
			masses[a] = new Mass(m);				
	}

	virtual void release()							
	{
		for (int a = 0; a < numOfMasses; ++a)		
		{
			delete(masses[a]);
			masses[a] = NULL;
		}
			
		delete(masses);
		masses = NULL;
	}

	Mass* getMass(int index)						// ���ص�i������
	{
		if (index < 0 || index >= numOfMasses)		
			return NULL;							

		return masses[index];						
	}

	virtual void init()								// ��ʼ�����е�����
	{
		for (int a = 0; a < numOfMasses; ++a)		
			masses[a]->init();						
	}

	virtual void solve()							// �麯�����ھ����Ӧ�������ø���ʩ�Ӹ������������
	{
													
	}

	virtual void simulate(float dt)					// �����е�����ģ��һ��
	{
		for (int a = 0; a < numOfMasses; ++a)		
			masses[a]->simulate(dt);				
	}

	virtual void operate(float dt)					// ������ģ�����
	{
		init();										// ������Ϊ0
		solve();									// Ӧ����
		simulate(dt);								// ģ��
	}

};

// ���к㶨�ٶȵ�����
class ConstantVelocity : public Simulation
{
public:
	ConstantVelocity() : Simulation(1, 1.0f)				//���캯��
	{
		masses[0]->pos = Vector3D(0.0f, 0.0f, 0.0f);		
		masses[0]->vel = Vector3D(1.0f, 0.0f, 0.0f);		
	}

};

// ���к㶨���ٵ�����
class MotionUnderGravitation : public Simulation
{
public:
	Vector3D gravitation;													//���ٶ�

	MotionUnderGravitation(Vector3D gravitation) : Simulation(1, 1.0f)		//���캯��
	{																		
		this->gravitation = gravitation;									//���ü��ٶ�
		masses[0]->pos = Vector3D(-10.0f, 0.0f, 0.0f);						//����λ��Ϊ���-10��
		masses[0]->vel = Vector3D(10.0f, 15.0f, 0.0f);						//�����ٶ�Ϊ����
	}

	virtual void solve()													//���õ�ǰ����
	{
		for (int a = 0; a < numOfMasses; ++a)								
			masses[a]->applyForce(gravitation * masses[a]->m);				
	}
	
};

// ����һ���ܵ����������ȵ���������
class MassConnectedWithSpring : public Simulation
{
public:
	float springConstant;													//����ϵ��
	Vector3D connectionPos;													//���ӷ���

	MassConnectedWithSpring(float springConstant) : Simulation(1, 1.0f)		//���캯��
	{
		this->springConstant = springConstant;								

		connectionPos = Vector3D(0.0f, -5.0f, 0.0f);						

		masses[0]->pos = connectionPos + Vector3D(10.0f, 0.0f, 0.0f);		
		masses[0]->vel = Vector3D(0.0f, 0.0f, 0.0f);						
	}

	virtual void solve()													//���õ�ǰ����
	{
		for (int a = 0; a < numOfMasses; ++a)								
		{
			Vector3D springVector = masses[a]->pos - connectionPos;			
			masses[a]->applyForce(-springVector * springConstant);			
		}
	}
	
};
