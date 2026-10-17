#include <Geode/Geode.hpp>
#include <Geode/modify/CCParticleSystemQuad.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/HardStreak.hpp>

using namespace geode::prelude;

// Only visual stuff is touched. Physics, triggers, hitboxes, TPS stay untouched.

static bool opt(char const* key) {
    return Mod::get()->getSettingValue<bool>(key);
}

static bool active() {
    if (!opt("only-in-level")) return true;
    return PlayLayer::get() != nullptr;
}

class $modify(LEOParticle, CCParticleSystemQuad) {
    void draw() {
        if (opt("disable-particles") && active()) return;
        CCParticleSystemQuad::draw();
    }
};

class $modify(LEOStreak, CCMotionStreak) {
    void draw() {
        if (opt("disable-streak") && active()) return;
        CCMotionStreak::draw();
    }
};

class $modify(LEOBase, GJBaseGameLayer) {
    CCParticleSystemQuad* createParticle(int id, char const* plist, int zOrder, tCCPositionType type) {
        auto p = GJBaseGameLayer::createParticle(id, plist, zOrder, type);
        if (p && active()) {
            int pct = static_cast<int>(Mod::get()->getSettingValue<int64_t>("particle-amount"));
            if (pct < 100) {
                p->setEmissionRate(p->getEmissionRate() * static_cast<float>(pct) / 100.f);
            }
        }
        return p;
    }

    void shakeCamera(float duration, float strength, float interval) {
        if (opt("disable-shake") && active()) return;
        GJBaseGameLayer::shakeCamera(duration, strength, interval);
    }
};

class $modify(LEOPlayer, PlayerObject) {
    void playDeathEffect() {
        if (opt("disable-death-effect") && active()) return;
        PlayerObject::playDeathEffect();
    }
};

class $modify(LEOWave, HardStreak) {
    void updateStroke(float dt) {
        if (opt("disable-wave-trail") && active()) return;
        HardStreak::updateStroke(dt);
    }
};
