#pragma once
#include "block.hpp"

// 7 classic tetrominoes
class LBlock : public Block { public: LBlock(); };
class JBlock : public Block { public: JBlock(); };
class IBlock : public Block { public: IBlock(); };
class OBlock : public Block { public: OBlock(); };
class SBlock : public Block { public: SBlock(); };
class TBlock : public Block { public: TBlock(); };
class ZBlock : public Block { public: ZBlock(); };

// 2 pentominoes (5 blocks) as bonus pieces
class PBlock : public Block { public: PBlock(); };
class UBlock : public Block { public: UBlock(); };
