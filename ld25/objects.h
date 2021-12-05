class object : public bzsf::Drawable {
protected:
	int layer;
	static int g_ID;
	int id;

public:
	int GetLayer()				{return layer;	}
	int GetID()					{return id;		}

	object() {
		layer = 0;
		id = object::g_ID;
		object::g_ID++;
	}

	virtual void activate() {}
	virtual void deactivate() {}

	virtual void draw() {
		bzsflegacy::game::window->draw(*this);
	}

	virtual ~object()	{}
};
int object::g_ID = 0;


class wall : public object {
public:
	wall(bzsf::tsTile * Tile, float xPos, float yPos, int l) {
		layer = l;
		setTile(Tile);
		setPosition(xPos, yPos);

		auto r = getTextureRect();
		vector<tbc::cdTile *> vec = tbc::SetTilesTaken((int)xPos, (int)yPos, (int)xPos+r.width, (int)yPos+r.height, "immovable", id);
	}

	~wall() {
		auto r = getTextureRect();
		tbc::UnSetTilesTaken((int)getPosition().x, (int)getPosition().y, (int)getPosition().x+(int)(getScale().x*r.width), (int)getPosition().y+(int)(getScale().y*r.height), "immovable");
	}
};

class helper : public object {
protected:
	bool displayText;
	string message;
	vector<tbc::cdTile *> myTiles;
	sf::Text text;

	bzsf::Animation * myAnim;

public:
	void activate() {
		tbc::UnSetTilesTaken(myTiles, "helper", id);
		displayText = true;
	}

	void deactivate() {
		displayText = false;
		myTiles = tbc::SetTilesTaken((int)getPosition().x, (int)getPosition().y, (int)getPosition().x+getLocalBounds().width, (int)getPosition().y+getLocalBounds().height, "helper", id);
	}

	helper(string _m, float xPos, float yPos, int l) : message(_m) {
		layer = l;
		myAnim = new bzsf::Animation(sf::Vector2u(32, 32), helperTexture, sf::milliseconds(100), true);
		setAnimation(*myAnim);
		setPosition(xPos, yPos);

		text = sf::Text(message, helperFont, 12);
		text.setOrigin(text.getLocalBounds().width/2, 0);
		text.setPosition(getPosition().x+16, getPosition().y-50);
		displayText = false;


		deactivate();
	}

	void draw() {
		assert(bzsflegacy::game::window != nullptr);

		bzsflegacy::game::window->draw(*this);
		if(displayText) {bzsflegacy::game::window->draw(text);}
	}

	~helper() {
		delete myAnim;
	}
};


class death : public object {
protected:
	bzsf::Animation * myAnim;

public:
	death(float xPos, float yPos, int l) {
		layer = l;
		myAnim = new bzsf::Animation(sf::Vector2u(32, 32), deathTexture, sf::milliseconds(80+rand()%40), true);
		setAnimation(*myAnim);
		setPosition(xPos, yPos);

		tbc::SetTilesTaken((int)getPosition().x, (int)getPosition().y, (int)getPosition().x+getLocalBounds().width, (int)getPosition().y+getLocalBounds().height, "death", id);
	}

	~death() {
		delete myAnim;
	}
};

class finish : public object {
protected:
	bzsf::Animation * myAnim;

public:
	finish(float xPos, float yPos, int l) {
		layer = l;
		myAnim = new bzsf::Animation(sf::Vector2u(32, 32), finishTexture, sf::milliseconds(150), true);
		setAnimation(*myAnim);
		setPosition(xPos, yPos);

		tbc::SetTilesTaken((int)getPosition().x, (int)getPosition().y, (int)getPosition().x+getLocalBounds().width, (int)getPosition().y+getLocalBounds().height, "finish", id);
	}

	~finish() {
		delete myAnim;
	}
};

class player : public object {
protected:
	//Hastighed på spriten
	float maxSpeed;
	float curSpeedY;
	float curSpeedX;

	float speedAcceleration;
	float speedDeceleration;
	float realSpeedX;
	float realSpeedY;

	float jumpStrength;
	float gravity;
	bool inAir;

	//Overflow variabler
	float posXOver;
	float posYOver;
	float negXOver;
	float negYOver;

	sf::Clock teleportClock;


	bool isTeleporting;
	bool isStartingTeleport;
	bool isFinishingTeleport;
	float teleportPosCounter;
	sf::Rect<float> teleportTargetPos;
	sf::Rect<float> finishTargetPos;

	bzsf::Animation * tpTargetAnim;
	bzsf::Drawable teleportTarget;

	bool isDying;
	bool isDead;

	bool isFinishing;

	object * currentHelper;

	//Overflow
	void speedOverflow(int type) {
		switch(type) {
			case 0:
			if(realSpeedX>0) {
				posXOver += realSpeedX - floor(realSpeedX);
				realSpeedX = floor(realSpeedX);
			}

			if(posXOver>=1) {
				realSpeedX++;
				posXOver--;
			} else {
				negXOver += realSpeedX - ceil(realSpeedX);
				realSpeedX = ceil(realSpeedX);

				if(negXOver<=-1) {
					realSpeedX--;
					negXOver++;
				}

				return;
			}
			break;

			case 1:
			if(realSpeedY>0) {
				posYOver += realSpeedY - floor(realSpeedY);
				realSpeedY = floor(realSpeedY);
			}

			if(posYOver>=1) {
				realSpeedY++;
				posYOver--;
			} else {
				negYOver += realSpeedY - ceil(realSpeedY);
				realSpeedY = ceil(realSpeedY);

				if(negYOver<=-1) {
					realSpeedY--;
					negYOver++;
				}

				return;
			}
			break;
		}
	}

	void move(float mDelta) {
		realSpeedX = curSpeedX * mDelta;
		realSpeedY = curSpeedY * mDelta;
		speedOverflow(0);//speedOverflow(1);


		if(!tbc::CheckCollision(this, ((curSpeedX<0) ? -8 : 8), 0, "immovable")) {
			setPosition(getPosition().x+realSpeedX, getPosition().y); //Spriten bevæger sig altid, dog kun med en hastighed over 0 hvis W er trykket på.
		} else {
			vector<tbc::cdTile *> lastCols = tbc::GetLastCollisions();
			tbc::cdTile * lastCol = lastCols[0];
			pair<vector<string *> *, vector<int> *> p = lastCol->GetInfo();
 			vector<int> occupierIDs = *p.second;
			object * obj = findObject(occupierIDs[0]);

			setPosition(obj->getPosition().x + ((curSpeedX<0) ? 32+(getLocalBounds().width*(getOrigin().x/getLocalBounds().width)) : -(getLocalBounds().width*(getOrigin().x/getLocalBounds().width))), getPosition().y);
			curSpeedX = 0;
		}



		vector<tbc::cdTile *> cArray = tbc::CollisionArray(this, 0, (curSpeedY>=0) ? 8 : -8, true);
		bool objBelow = false;
		if(cArray.size() > 0) {
			bool found = false;
			for(unsigned int i=0; i < cArray.size() && !found; i++) {
				pair<vector<string *> *, vector<int> *> p = cArray[i]->GetInfo();
				vector<string *> occupiers = *p.first;
 				vector<int> occupierIDs = *p.second;

				for(unsigned int k=0; k < occupiers.size(); k++) {
					if(occupiers[k]->compare("immovable") == 0) {
						object * obj = findObject(occupierIDs[k]);
						if(curSpeedY >=0) {
							inAir = false;
							objBelow = true;
							curSpeedY = 0;

							setPosition(getPosition().x, obj->getPosition().y-(getLocalBounds().height*(getOrigin().y/getLocalBounds().height)));
							found = true;
						} else {
							curSpeedY = 0;

							setPosition(getPosition().x, obj->getPosition().y + 32+(getLocalBounds().height*(getOrigin().y/getLocalBounds().height)));
							found = true;
						}
						break;
					}
				}
			}
		}

		if(!objBelow) {
			inAir = true;
		}


		if(inAir) {
			curSpeedY += gravity*mDelta;
		}


		realSpeedY = curSpeedY * mDelta;speedOverflow(1);
		setPosition(getPosition().x, getPosition().y + round(realSpeedY));
	}

	void speedToZero(float mDelta) {
		if(curSpeedX>0) {
			if(curSpeedX<speedDeceleration*mDelta) {
				curSpeedX = 0;
			} else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				curSpeedX -= speedDeceleration*mDelta;
			}
		} else if(curSpeedX<0) {
			if(curSpeedX>speedDeceleration*mDelta) {
				curSpeedX = 0;
			} else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				curSpeedX += speedDeceleration*mDelta;
			}
		}
	}


		//Movement
	void checkInput(float mDelta) {
		//Jump
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !inAir) {
			if(!tbc::CheckCollision(this, 0, -32, "immovable")) {
				curSpeedY = -jumpStrength;
			}
		}

		//Move left
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && curSpeedX>-maxSpeed) {
			if(curSpeedX-(speedAcceleration*mDelta)<-maxSpeed) {
				curSpeedX = -maxSpeed;
			}
			else {curSpeedX -= speedAcceleration*mDelta;}
		}

		//Move right
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && curSpeedX<maxSpeed) {
			if((curSpeedX+(speedAcceleration*mDelta))>maxSpeed) {
				curSpeedX = maxSpeed;
			}
			else {curSpeedX += speedAcceleration*mDelta;}
		}
	}


public:
	float GetCurSpeedX()	{return curSpeedX;		}
	float GetCurSpeedY()	{return curSpeedY;		}
	bool IsInAir()			{return inAir;			}
	bool IsTeleporting()		{return isTeleporting;		}
	bool IsStartingTeleport()	{return isStartingTeleport;	}
	bool IsFinishingTeleport()	{return isFinishingTeleport;}
	bool IsDead()				{return isDead;				}
	bool IsFinishing()			{return isFinishing;		}
	bzsf::Drawable& GetTeleportTarget() {return teleportTarget;}
	sf::FloatRect GetOverflow() {return sf::FloatRect(posXOver, negXOver, posYOver, negYOver);}

	virtual void hit() {}

	void tick(const float mDelta, sf::View& view) {
		if(!isDead) {
			if(!isTeleporting && !isDying && !isFinishing) {
				checkInput(mDelta);

				speedToZero(mDelta);
				move(mDelta);

				view.setCenter(getPosition().x, getPosition().y);

				if(tbc::CheckCollision(this, 0, 0, "death")) {
					electroSnd.play();
					die();
				}

				setRotation(DEGTORAD((curSpeedX/maxSpeed)*1000));
				if(curSpeedX<0) {
					setScale(-1.f, 1);
				} else if (curSpeedX > 40.f) {
					setScale(1.f, 1);
				}

				if(currentHelper != 0) {
					if((getPosition().x > currentHelper->getPosition().x+64) || (getPosition().x < currentHelper->getPosition().x-32)) {
						currentHelper->deactivate();
						currentHelper = 0;
					}
				} else {
					if(tbc::CheckCollision(this, 0, 0, "helper")) {
						vector<int> vec = *tbc::GetLastCollisions()[0]->GetInfo().second;
						currentHelper = findObject(vec[0]);
						currentHelper->activate();
					}
				}

				if(tbc::CheckCollision(this, 0, 0, "finish")) {
					isFinishing = true;
					teleportClock.restart();
					finishTargetPos.left = Finish->getPosition().x+16;
					finishTargetPos.top = Finish->getPosition().y+16;
					finishTargetPos.width = getPosition().x;
					finishTargetPos.height = getPosition().y;
				}
			} else if(!isDying && !isFinishing) {
				/////////////////////////////////////////////////////////////////////////////////////
				float elapsedTime = teleportClock.getElapsedTime().asSeconds()*3; // this should always be a number that goes from 0 - 1
				if(isStartingTeleport) {
					if(elapsedTime < 1) {
						setScale(((getScale().x<0) ? -1.f+elapsedTime*0.8f : 1.f-elapsedTime*0.8f) , 1.f-elapsedTime*0.8f);
						teleportTarget.setScale(elapsedTime, elapsedTime);
					} else {
						isStartingTeleport = false;
					}
				} else if(isFinishingTeleport) {
					if(elapsedTime < 1) {
						setScale(((getScale().x<0) ? -0.2f-elapsedTime*0.8f : 0.2f+elapsedTime*0.8f) , 0.2f+elapsedTime*0.8f);

						float xDistance = teleportTargetPos.width-teleportTargetPos.left;
						float yDistance = teleportTargetPos.height-teleportTargetPos.top;
						setPosition( teleportTargetPos.left + xDistance*elapsedTime, teleportTargetPos.top + yDistance*elapsedTime);
						view.setCenter(getPosition().x, getPosition().y);
					} else {
						setPosition(teleportTargetPos.width, teleportTargetPos.height);
						if(tbc::CheckCollision(this, 0, 0, "immovable")) {
							die();
						}
						isFinishingTeleport = false;
						isTeleporting = false;
						getAnimation()->setTimePerFrame(sf::milliseconds(166));
					}
				} else {
					teleportPosCounter += mDelta*300;
					teleportTarget.setPosition(getPosition().x + ((getScale().x<0) ? -1 : 1)*(100 + sin(DEGTORAD(teleportPosCounter))*75), getPosition().y);

					float distance = teleportTarget.getPosition().x-getPosition().x;
					bool collides = tbc::CheckCollision(this, distance, 0, "immovable");

					if(collides) {
						float yDistance1 = (round(getPosition().y/16)*16)-16 - getPosition().y;
						float yDistance2 = (floor(getPosition().y/32)*32)-16 - getPosition().y;
						float yDistance3 = (ceil(getPosition().y/32)*32)+16  - getPosition().y;
						float yDistance4 = (round(getPosition().y/16)*16)+16 - getPosition().y;
						float yDistance5 = (round(getPosition().y/16)*16)    - getPosition().y;
						if(!tbc::CheckCollision(this, distance, floor(yDistance5), "immovable")) {
							//cout << "case 5";
							collides = false;
							teleportTarget.setPosition(teleportTarget.getPosition().x, (round(getPosition().y/16)*16));
						} else if(!tbc::CheckCollision(this, distance, floor(yDistance1), "immovable")) {
							//cout << "case 1";
							collides = false;
							teleportTarget.setPosition(teleportTarget.getPosition().x, (round(getPosition().y/16)*16)-16);
						}  else if (!tbc::CheckCollision(this, distance, floor(yDistance4), "immovable")) {
							//cout << "case 4";
							collides = false;
							teleportTarget.setPosition(teleportTarget.getPosition().x, (round(getPosition().y/16)*16)+16);
						} else if (!tbc::CheckCollision(this, distance, floor(yDistance2), "immovable")) {
							//cout << "case 2";
							collides = false;
							teleportTarget.setPosition(teleportTarget.getPosition().x, (floor(getPosition().y/32)*32)-16);
						} else if (!tbc::CheckCollision(this, distance, floor(yDistance3), "immovable")) {
							//cout << "case 3";
							collides = false;
							teleportTarget.setPosition(teleportTarget.getPosition().x, (ceil(getPosition().y/32)*32)+16);
						}
					}

					if(!collides) {
						teleportTarget.setColor(sf::Color(103, 232, 9, 255));
					} else {
						teleportTarget.setColor(sf::Color(255, 5, 0, 255));
					}
				}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			} else if(!isFinishing) { // The Player is dying
				float elapsedTime = teleportClock.getElapsedTime().asSeconds()*1; // this should always be a number that goes from 0 - 1
				if(elapsedTime < 1) {
					setScale(1.f + elapsedTime, 1.f + elapsedTime);
					setRotation(elapsedTime*720.f);
					setColor(sf::Color(255, 255.f-elapsedTime*255.f, 255.f-elapsedTime*255.f, 255));
				} else {
					bzsf::Animation * bloodAnim = new bzsf::Animation(sf::Vector2u(64, 64), bloodTexture, sf::milliseconds(30), false);
					setOrigin(32, 32);
					setAnimation(*bloodAnim);
					isDying = false;
					isDead = true;
				}
			} else { //the player should be dragged into the portal
				float elapsedTime = teleportClock.getElapsedTime().asSeconds()*0.5;
				if(elapsedTime < 1) {
					float xDistance = finishTargetPos.left - finishTargetPos.width;
					float yDistance = finishTargetPos.top - finishTargetPos.height;

					setPosition(finishTargetPos.width + xDistance*elapsedTime, finishTargetPos.height + yDistance*elapsedTime);
					setScale(1.f - elapsedTime, 1.f - elapsedTime);
					setRotation(2000*elapsedTime);
				} else {
					setPosition(Finish->getPosition().x+16, Finish->getPosition().y+16);

					stringstream levelstream;
					levelstream << "levels\\" << level[0] << "-" << level[1]+1 << ".txt";

					ifstream file(levelstream.str().c_str());
					if(file) {
						level[1] = level[1]+1;
					} else {bzsflegacy::game::currentState = BOSS;}
				}
			}
		}
	}

	void teleport() {
		isTeleporting = true;
		isStartingTeleport = true;
		teleportTarget.setColor(sf::Color(255, 255, 255, 255));
		teleportTarget.setScale(0.f, 0.f);
		teleportTarget.setPosition(getPosition().x + ((getScale().x<0) ? -1 : 1)*(100 + sin(DEGTORAD(teleportPosCounter))*75), getPosition().y);

		teleportClock.restart();

		getAnimation()->setTimePerFrame(sf::Time::Zero);

		teleportSnd2.play();
	}

	void finishTeleport() {
		isFinishingTeleport = true;

		//curSpeedX = curSpeedX/5;
		curSpeedY = 0;
		teleportPosCounter = 270;
		teleportTargetPos.left = getPosition().x;
		teleportTargetPos.width = teleportTarget.getPosition().x;
		teleportTargetPos.top = getPosition().y;
		teleportTargetPos.height = teleportTarget.getPosition().y;
		teleportTarget.setColor(sf::Color(255, 255, 255, 0));

		teleportClock.restart();

		teleportSnd.play();
	}

	void die() {
		isDying = true;
		getAnimation()->setTimePerFrame(sf::milliseconds(50));
		teleportClock.restart();

		generalClock.restart();
	}

	player(sf::Texture& texture, float xPos, float yPos, int l) {
		layer = l;

		setTexture(texture);
		setPosition(xPos, yPos);

		//Hastighed på spriten
		maxSpeed			= 400.0f;
		curSpeedY			= 0.0f;
		curSpeedX			= 0.0f;

		jumpStrength		= 300.f;
		gravity				= 400.f;
		inAir				= false;

		speedAcceleration	= 1600.0f;
		speedDeceleration	= 1560.0f;

		posXOver = 0;
		posYOver = 0;
		negXOver = 0;
		negYOver = 0;

		currentHelper = 0;

		isDying = false;
		isDead = false;
		isFinishing = false;

		isTeleporting = false;
		isStartingTeleport = false;
		isFinishingTeleport = false;
		teleportTargetPos = sf::Rect<float>(0,0,0,0);
		finishTargetPos = sf::Rect<float>(0,0,0,0);
		teleportPosCounter = 270;

		tpTargetAnim = new bzsf::Animation(sf::Vector2u(24, 24), teleportTargetTexture, sf::milliseconds(50), true);
		teleportTarget.setAnimation(*tpTargetAnim);
		teleportTarget.setColor(sf::Color(255, 255, 255, 0));
		teleportTarget.setOrigin(teleportTarget.getLocalBounds().width/2, teleportTarget.getLocalBounds().height/2);
	}

	~player() {
		delete tpTargetAnim;
	}
};

vector<object *> objects;

bool sortObjects(object * o1, object * o2) {return (o1->GetLayer() > o2->GetLayer());}

void newObject(object * ptr) {
	objects.reserve(objects.size()+1);
	objects.push_back(ptr);
	sort(objects.begin(), objects.end(), sortObjects);
}

void clearObjects() {
	for(unsigned int i=0; i<objects.size(); i++) {
		delete objects[i];
	}
	objects.clear();
}

object * newObjectPtr(object * ptr) {
	objects.reserve(objects.size()+1);
	objects.push_back(ptr);
	sort(objects.begin(), objects.end(), sortObjects);
	return ptr;
}
player * newPlayerPtr(player * ptr) {
	objects.reserve(objects.size()+1);
	objects.push_back(ptr);
	return ptr;
}


object * findObject(unsigned int oID) {
	for(unsigned int i = 0; i<objects.size(); i++) {
		if(objects.at(i)->GetID() == oID) {
			return objects[i];
		}
	}
}


bool tbc::CheckCollision(player * Ent_obj, int addToX, int addToY , string sOccupier) { //This checks for a specific occupier
	int minX = (int)(Ent_obj->getPosition().x-(Ent_obj->getLocalBounds().width*(Ent_obj->getOrigin().x/Ent_obj->getLocalBounds().width)))+addToX;
	int minY = (int)(Ent_obj->getPosition().y-(Ent_obj->getLocalBounds().height*(Ent_obj->getOrigin().y/Ent_obj->getLocalBounds().height)))+addToY;
	int maxX = minX + (int)Ent_obj->getLocalBounds().width;
	int maxY = minY + (int)Ent_obj->getLocalBounds().height;


	return tbc::CheckCollision(minX, minY, maxX, maxY, sOccupier);
}

bool tbc::CheckCollision(sf::Sprite& Ent_obj, int addToX, int addToY , string sOccupier) { //This checks for a specific occupier
	int minX = (int)(Ent_obj.getPosition().x-(Ent_obj.getLocalBounds().width*(Ent_obj.getOrigin().x/Ent_obj.getLocalBounds().width)))+addToX;
	int minY = (int)(Ent_obj.getPosition().y-(Ent_obj.getLocalBounds().height*(Ent_obj.getOrigin().y/Ent_obj.getLocalBounds().height)))+addToY;
	int maxX = minX + (int)Ent_obj.getLocalBounds().width;
	int maxY = minY + (int)Ent_obj.getLocalBounds().height;


	return tbc::CheckCollision(minX, minY, maxX, maxY, sOccupier);
}

vector<tbc::cdTile *> tbc::CollisionArray(player * Ent_obj, int addToX, int addToY, bool OnlyReturnTaken) {
	/*int minX = (int)(Ent_obj->getPosition().x-((Ent_obj->GetTile()->width*Ent_obj->getScale().x)*(Ent_obj->getOrigin().x/Ent_obj->GetTile()->width))+addToX);
	int minY = (int)(Ent_obj->getPosition().y-((Ent_obj->GetTile()->height*Ent_obj->getScale().y)*(Ent_obj->getOrigin().y/Ent_obj->GetTile()->height))+addToY);
	int maxX = minX + (int)(Ent_obj->getScale().x*Ent_obj->GetTile()->width);
	int maxY = minY + (int)(Ent_obj->getScale().y*Ent_obj->GetTile()->height);*/

	int minX = (int)(Ent_obj->getPosition().x-(Ent_obj->getLocalBounds().width*(Ent_obj->getOrigin().x/Ent_obj->getLocalBounds().width)))+addToX;
	int minY = (int)(Ent_obj->getPosition().y-(Ent_obj->getLocalBounds().height*(Ent_obj->getOrigin().y/Ent_obj->getLocalBounds().height)))+addToY;
	int maxX = minX + (int)Ent_obj->getLocalBounds().width;
	int maxY = minY + (int)Ent_obj->getLocalBounds().height;


	return tbc::CollisionArray(minX, minY, maxX, maxY, OnlyReturnTaken);
}
