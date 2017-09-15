#ifndef PARTICLE_H
#define PARTICLE_H

class Partical
{
public:
    friend class ParticalSys;
    bool update(float time);
    Partical();
    ~Partical();
    void SetParent( ParticalSys* Parent) { m_pParent = Parent; }
private:


    ParticalSys* m_pParent;
};

class ParticalSys
{
public:
    ParticalSys();
    ~ParticalSys();
    Draw(float,float);
    Update(float time);
};

#endif // PARTICLE_H
