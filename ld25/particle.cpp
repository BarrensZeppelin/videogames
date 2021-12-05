#include "particle.h"
#include <sstream>

ParticleSystem::ParticleSystem( int width, int height )
{
	m_transparent = sf::Color( 0, 0, 0, 0 );
	m_image.create( width, height, m_transparent );
	sf::Texture text;
	text.loadFromImage(m_image);
	m_sprite.setTexture(text);

	m_position.x	= 0.5f * width;
	m_position.y	= 0.5f * height;
	m_particleSpeed	= 20.0f;
	m_dissolve	= false;
	m_dissolutionRate = 4;
	m_shape		= Shape::CIRCLE;
}

ParticleSystem::~ParticleSystem()
{
	for( ParticleIterator it = m_particles.begin(); it != m_particles.end(); it++ )
	{
		delete *it;
	}
}

void ParticleSystem::fuel( int particles )
{
	float angle;
	Particle* particle;
	for( int i = 0; i < particles; i++ )
	{
		particle = new Particle();
		particle->pos.x = m_position.x;
		particle->pos.y = m_position.y;
		
		switch( m_shape )
		{
		case Shape::CIRCLE:
			angle = float(rand()%62832)/1000;//m_randomizer.Random( 0.0f, 6.2832f );
			std::cout << angle;
			particle->vel.x = (rand()%int(cos( angle )*10000))/10000;
			particle->vel.y = (rand()%int(sin( angle )*10000))/10000;
			break;
		case Shape::SQUARE:
			particle->vel.x = (rand()%2000)/1000 - 1;
			particle->vel.y = (rand()%2000)/1000 - 1;
			break;
		default:
			particle->vel.x = 0.5f; // Easily detected
			particle->vel.y = 0.5f; // Easily detected
		}

		if( particle->vel.x == 0.0f && particle->vel.y == 0.0f )
		{
			delete particle;
			continue;
		}

		particle->color.r = rand()%256; //m_randomizer.Random( 0, 255 );
		particle->color.g = rand()%256;
		particle->color.b = rand()%256;
		particle->color.a = 255;
		m_particles.push_back( particle );
	}
}

void ParticleSystem::update()
{
	float time = m_clock.getElapsedTime().asSeconds();
	m_clock.restart();

	for( ParticleIterator it = m_particles.begin(); it != m_particles.end(); it++ )
	{
		(*it)->vel.x += m_gravity.x * time;
		(*it)->vel.y += m_gravity.y * time;

		(*it)->pos.x += (*it)->vel.x * time * m_particleSpeed;
		(*it)->pos.y += (*it)->vel.y * time * m_particleSpeed;

		if( m_dissolve ) (*it)->color.a -= m_dissolutionRate;

		if( (*it)->pos.x > m_sprite.getTexture()->getSize().x || (*it)->pos.x < 0 || (*it)->pos.y > m_sprite.getTexture()->getSize().y || (*it)->pos.y < 0 || (*it)->color.a < 10 )
		{
			delete (*it);
			it = m_particles.erase( it );
			if( it == m_particles.end() ) return;
		}
	}
}

void ParticleSystem::render()
{
	for( ParticleIterator it = m_particles.begin(); it != m_particles.end(); it++ )
	{
		delete m_sprite.getTexture();
		sf::Texture text;
		m_image.setPixel( (int)(*it)->pos.x, (int)(*it)->pos.y, (*it)->color );
		text.loadFromImage(m_image);
		m_sprite.setTexture(text);
		
	}
}

void ParticleSystem::remove()
{
	for( ParticleIterator it = m_particles.begin(); it != m_particles.end(); it++ )
	{	
		delete m_sprite.getTexture();
		sf::Texture text;
		m_image.setPixel( (int)(*it)->pos.x, (int)(*it)->pos.y, m_transparent );
		text.loadFromImage(m_image);
		m_sprite.setTexture(text);
	}
}

std::string ParticleSystem::getNumberOfParticlesString()
{
	std::ostringstream oss;
	oss << m_particles.size();
	return oss.str();
}