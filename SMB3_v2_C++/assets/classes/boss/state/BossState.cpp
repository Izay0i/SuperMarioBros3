#include "../../GlobalUtil.h"
#include "BossState.h"

FortressBoss* BossState::_fortressBoss = nullptr;

BossState::BossState(FortressBoss* fortressBoss) {
	if (_fortressBoss == nullptr) {
		_fortressBoss = fortressBoss;
	}
}

BossState::~BossState() {}

void BossState::Update(DWORD) {}

void BossState::Render() {}

void BossState::Release() {
	_fortressBoss = nullptr;
}