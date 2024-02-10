#include <memory>

#include "./include/puzzle.h"
#include "./include/utils.h"

// (1) exp → exp + term
// (2) exp → exp – term
// (3) exp → term
// (4) term → term * fact
// (5) term → term / fact
// (6) term → fact
// (7) fact → ID
// (8) fact → ( exp )

// type id = ID of string

// type exp =
//   | Plus of exp * term
//   | Minus of exp * term
//   | Term of term

// and term =
//   | Times of term * fact
//   | Divide of term * fact
//   | Fact of fact

// and fact =
//   | Identifier of id
//   | Parentheses of exp

struct Id;
struct Exp;
struct Term;
struct Fact;

struct Node {
  virtual ~Node() = default;
};

struct Id : Node {
  std::string name;
  explicit Id(const std::string &n) : name(n) {}
};

struct Exp : Node {
  virtual ~Exp() = default;
};

struct Plus : Exp {
  std::unique_ptr<Exp> left;
  std::unique_ptr<Term> right;
  Plus(std::unique_ptr<Exp> l, std::unique_ptr<Term> r) : left(std::move(l)), right(std::move(r)) {}
};

struct Minus : Exp {
  std::unique_ptr<Exp> left;
  std::unique_ptr<Term> right;
  Minus(std::unique_ptr<Exp> l, std::unique_ptr<Term> r) : left(std::move(l)), right(std::move(r)) {}
};

struct Term : Node {
  virtual ~Term() = default;
};

struct Times : Term {
  std::unique_ptr<Term> left;
  std::unique_ptr<Fact> right;
  Times(std::unique_ptr<Term> l, std::unique_ptr<Fact> r) : left(std::move(l)), right(std::move(r)) {}
};

struct Divide : Term {
  std::unique_ptr<Term> left;
  std::unique_ptr<Fact> right;
  Divide(std::unique_ptr<Term> l, std::unique_ptr<Fact> r) : left(std::move(l)), right(std::move(r)) {}
};

struct Fact : Node {
  virtual ~Fact() = default;
};

struct Identifier : Fact {
  std::unique_ptr<Id> id;
  explicit Identifier(std::unique_ptr<Id> i) : id(std::move(i)) {}
};

struct Parens : Fact {
  std::unique_ptr<Exp> exp;
  explicit Parens(std::unique_ptr<Exp> e) : exp(std::move(e)) {}
};

Puzzle ast_puzzle_create(long seed)
{
  (void)seed;
  return {};
}
