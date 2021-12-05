bool LineIntersectsLine(sf::Vector2f l1p1, sf::Vector2f l1p2, sf::Vector2f l2p1, sf::Vector2f l2p2) {
    float q = (l1p1.y - l2p1.y) * (l2p2.x - l2p1.x) - (l1p1.x - l2p1.x) * (l2p2.y - l2p1.y);
    float d = (l1p2.x - l1p1.x) * (l2p2.y - l2p1.y) - (l1p2.y - l1p1.y) * (l2p2.x - l2p1.x);

    if( d == 0 )
    {
        return false;
    }

    float r = q / d;

    q = (l1p1.y - l2p1.y) * (l1p2.x - l1p1.x) - (l1p1.x - l2p1.x) * (l1p2.y - l1p1.y);
    float s = q / d;

    if( r < 0 || r > 1 || s < 0 || s > 1 )
    {
        return false;
    }

    return true;
}

bool LineIntersectsRect(sf::Vector2f p1, sf::Vector2f p2, sf::FloatRect r) {
    return LineIntersectsLine(p1, p2, sf::Vector2f(r.left, r.top), sf::Vector2f(r.left + r.width, r.top)) ||
           LineIntersectsLine(p1, p2, sf::Vector2f(r.left + r.width, r.top), sf::Vector2f(r.left + r.width, r.top + r.height)) ||
		   LineIntersectsLine(p1, p2, sf::Vector2f(r.left + r.width, r.top + r.height), sf::Vector2f(r.left, r.top + r.height)) ||
           LineIntersectsLine(p1, p2, sf::Vector2f(r.left, r.top + r.height), sf::Vector2f(r.left, r.top)) ||
		   (r.contains(p1) && r.contains(p2));
}

