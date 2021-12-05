#include "CliffNode.hpp"
#include "Utility.hpp"
#include "Application.hpp"

CliffNode::CliffNode(const TextureStorage& textures, const int SEALINE) : SceneNode(Category::Cliff) {
	top.setTexture(textures.get(Texture::TopCliff));
	bottom.setTexture(textures.get(Texture::BottomCliff));
	bottom.setPosition(0, SEALINE);
	
	
	// Initalize triangles (hardcoded)
	tris[0].size = sf::FloatRect(78, 0, 36, 88);
	tris[0].normal = UnitVector(sf::Vector2f(tris[0].size.height,
								(tris[0].size.width - tris[0].size.left)));

	sf::Vector2f start(50 + 64, 88);
	sf::Vector2f end(50, 88 + 33);
	sf::Vector2f normal(end.y - start.y, -(end.x - start.x));
	tris[1].size = sf::FloatRect(50, 88, 64, 33);
	tris[1].normal = UnitVector(normal);

	start = sf::Vector2f(50, 119);
	end = sf::Vector2f(50 + 10, 119 + 53);
	normal = sf::Vector2f(end.y - start.y, -(end.x - start.x));
	tris[2].size = sf::FloatRect(50, 119, 10, 53);
	tris[2].normal = UnitVector(normal);

	tris[3].size = sf::FloatRect(60, 166, 60, 12);
	tris[3].normal = sf::Vector2f(0, -1);

	tris[4].size = sf::FloatRect(112, 178, 8, 284 + 150);
	tris[4].normal = sf::Vector2f(1, 0);
}


void CliffNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) {	
	target.draw(top);
}


// Sloppy SAT implementation
inline float pro(float halfWidth1, float halfWidth2, float left1, float left2) {
	float distance = std::abs((left2 + halfWidth2) - (left1 + halfWidth1));
	return (halfWidth1 + halfWidth2) - distance;
}

std::pair<bool, sf::Vector2f> CliffNode::testCollision(const sf::FloatRect& bbox) {
	if(bbox.left > RIGHT) // Not in range of cliff
		return std::make_pair(false, sf::Vector2f());
	
	sf::Vector2f nudge;

	for(const Triangle& tri : tris) {
		sf::Vector2f proVector(100, 100);
		float d = pro(tri.size.width / 2, bbox.width / 2, tri.size.left, bbox.left);
		if(d <= 0)
			continue;
		else {
			proVector = sf::Vector2f(d, 0);
		}

		/*if(tri.normal == sf::Vector2f(1, 0)) // Wall
			return std::make_pair(true, proVector);
			*/

		d = pro(tri.size.height / 2, bbox.height / 2, tri.size.top, bbox.top);
		if(d <= 0)
			continue;
		else if(d <= proVector.x)
			proVector = sf::Vector2f(0, d);

		if(tri.normal == sf::Vector2f(0, -1)) // Plane you can stand on
			return std::make_pair(true, sf::Vector2f(proVector.x, -proVector.y));


		float trimin, trimax, rectmin, rectmax;
		
		if(tri.normal.y < 0) {
			trimin = tri.size.left * tri.normal.x + ((tri.size.top + tri.size.height)) * tri.normal.y;
			trimax = tri.size.left * tri.normal.x + (tri.size.top) * tri.normal.y;

			rectmin = bbox.left * tri.normal.x + ((bbox.top + bbox.height)) * tri.normal.y;
			rectmax = (bbox.left + bbox.width) * tri.normal.x + (bbox.top) * tri.normal.y;
		} else {
			trimin = tri.size.left * tri.normal.x + ((tri.size.top + tri.size.height)) * tri.normal.y;
			trimax = (tri.size.left + tri.size.width) * tri.normal.x + (tri.size.top) * tri.normal.y;

			rectmin = bbox.left * tri.normal.x + (bbox.top) * tri.normal.y;
			rectmax = (bbox.left + bbox.width) * tri.normal.x + (bbox.top + bbox.height) * tri.normal.y;
		}

		if(rectmin < trimax && trimin < rectmax) {
			float d = trimax - rectmin;
			sf::Vector2f dv(d * tri.normal.x, d * tri.normal.y);
			if(Length(dv) < Length(proVector))
				proVector = dv;
		} else
			continue;

		nudge += proVector;
	}

	return std::make_pair((nudge != sf::Vector2f() ? true : false), nudge);
}


sf::Uint32 CliffNode::getCategory() {
	return Category::Cliff;
}

void CliffNode::drawSea(sf::RenderTarget& target, sf::RenderStates states) {
	states.transform *= bottom.getTransform();

	target.draw(bottom);
}