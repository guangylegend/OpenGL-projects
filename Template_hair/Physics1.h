#include <math.h>

// 定义一个3D位置的类
class Vector3D
{
public:
	float x;									
	float y;									
	float z;									

	Vector3D()									// 初始化 x = y = z = 0
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3D(float x, float y, float z)			// 初始化
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3D& operator= (Vector3D v)			// 附值
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	Vector3D operator+ (Vector3D v)				// 加
	{
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}

	Vector3D operator- (Vector3D v)				// 减
	{
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}

	Vector3D operator* (float value)			// 乘
	{
		return Vector3D(x * value, y * value, z * value);
	}

	Vector3D operator/ (float value)			// 除
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

	float length()								//返回长度
	{
		return sqrtf(x*x + y*y + z*z);
	};			   		

	void unitize()								// 归一化
	{
		float length = this->length();

		if (length == 0)
			return;

		x /= length;
		y /= length;
		z /= length;
	}

	Vector3D unit()								// 返回单位向量
	{
		float length = this->length();

		if (length == 0)
			return *this;
		
		return Vector3D(x / length, y / length, z / length);
	}

};

// 表示一个物体
class Mass
{
public:
	float m;									// 质量
	Vector3D pos;								// 位置
	Vector3D vel;								// 速度
	Vector3D force;								// 力

	Mass(float m)								// 构造函数
	{
		this->m = m;
	}

	// 增加一个力
	void applyForce(Vector3D force)
	{
		this->force += force;				
	}

	// 初始时设置为0
	void init()
	{
		force.x = 0;
		force.y = 0;
		force.z = 0;
	}

	//根据输入的时间dt，模拟力的效果
	void simulate(float dt)
	{
		vel += (force / m) * dt;			
											

		pos += vel * dt;					
											
	}

};

// 模拟类
class Simulation
{
public:
	int numOfMasses;								// 物体的个数
	Mass** masses;									// 指向物体结构的指针
	
	Simulation(int numOfMasses, float m)			// 构造函数
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

	Mass* getMass(int index)						// 返回第i个物体
	{
		if (index < 0 || index >= numOfMasses)		
			return NULL;							

		return masses[index];						
	}

	virtual void init()								// 初始化所有的物体
	{
		for (int a = 0; a < numOfMasses; ++a)		
			masses[a]->init();						
	}

	virtual void solve()							// 虚函数，在具体的应用中设置各个施加给各个物体的力
	{
													
	}

	virtual void simulate(float dt)					// 让所有的物体模拟一步
	{
		for (int a = 0; a < numOfMasses; ++a)		
			masses[a]->simulate(dt);				
	}

	virtual void operate(float dt)					// 完整的模拟过程
	{
		init();										// 设置力为0
		solve();									// 应用力
		simulate(dt);								// 模拟
	}

};

// 具有恒定速度的物体
class ConstantVelocity : public Simulation
{
public:
	ConstantVelocity() : Simulation(1, 1.0f)				//构造函数
	{
		masses[0]->pos = Vector3D(0.0f, 0.0f, 0.0f);		
		masses[0]->vel = Vector3D(1.0f, 0.0f, 0.0f);		
	}

};

// 具有恒定加速的物体
class MotionUnderGravitation : public Simulation
{
public:
	Vector3D gravitation;													//加速度

	MotionUnderGravitation(Vector3D gravitation) : Simulation(1, 1.0f)		//构造函数
	{																		
		this->gravitation = gravitation;									//设置加速度
		masses[0]->pos = Vector3D(-10.0f, 0.0f, 0.0f);						//设置位置为左边-10处
		masses[0]->vel = Vector3D(10.0f, 15.0f, 0.0f);						//设置速度为右上
	}

	virtual void solve()													//设置当前的力
	{
		for (int a = 0; a < numOfMasses; ++a)								
			masses[a]->applyForce(gravitation * masses[a]->m);				
	}
	
};

// 创建一个受到与距离成正比的力的物体
class MassConnectedWithSpring : public Simulation
{
public:
	float springConstant;													//弹性系数
	Vector3D connectionPos;													//连接方向

	MassConnectedWithSpring(float springConstant) : Simulation(1, 1.0f)		//构造函数
	{
		this->springConstant = springConstant;								

		connectionPos = Vector3D(0.0f, -5.0f, 0.0f);						

		masses[0]->pos = connectionPos + Vector3D(10.0f, 0.0f, 0.0f);		
		masses[0]->vel = Vector3D(0.0f, 0.0f, 0.0f);						
	}

	virtual void solve()													//设置当前的力
	{
		for (int a = 0; a < numOfMasses; ++a)								
		{
			Vector3D springVector = masses[a]->pos - connectionPos;			
			masses[a]->applyForce(-springVector * springConstant);			
		}
	}
	
};
