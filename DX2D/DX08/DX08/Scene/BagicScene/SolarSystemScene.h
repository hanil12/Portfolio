#pragma once
class SolarSystemScene : public Scene
{
public:
	SolarSystemScene();
	virtual ~SolarSystemScene();

	// Scene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	shared_ptr<Planet> _sun;
	shared_ptr<Planet> _earth;
	shared_ptr<Planet> _moon;
};

