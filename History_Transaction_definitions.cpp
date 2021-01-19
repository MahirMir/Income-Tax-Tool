#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK
//
Transaction::Transaction(std::string ticker_symbol, unsigned int day_date,
					unsigned int month_date, unsigned int year_date,
					bool buy_sell_trans, unsigned int number_shares,
					double trans_amount) {
	trans_id = assigned_trans_id;
	assigned_trans_id++;
	symbol = ticker_symbol;
	day = day_date;
	month = month_date;
	year = year_date;
	trans_type = (buy_sell_trans) ? "Buy" : "Sell";
	shares = number_shares;
	amount = trans_amount;
	p_next = nullptr;

	//
	acb = 0;
	share_balance = 0;
	cgl = 0;
	acb_per_share = 0;
	//
}


// Destructor
// TASK
//
Transaction::~Transaction() {
	//std::cout << "T";
}

// TASK
// Overloaded < operator.
//
bool Transaction::operator<(Transaction const &other) {

	Transaction const *current{this};
	Transaction const *next{&other};
	//std::cout << current->symbol << std::endl;
	if (current->year < next->year) { //Return true if current < next
		return true;
	} else if ((current->month < next->month) && (current->year == next->year)) {
		return true;
	} else if ((current->day < next->day) && (current->month == next->month) && (current->year == next->year)) {
		return true;
	} else return false;
}
// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK
//
History::History() {
	p_head = nullptr;
}

// Destructor
// TASK
//
History::~History() {
	//std::cout << "H";
	Transaction *p_temp{nullptr};
	while (p_head != nullptr) {
		p_temp = p_head;
		p_head = p_head->get_next();
		delete p_temp;
		p_temp = nullptr;
	}

}

// TASK
// read_history(...): Read the transaction history from file.
//
void History::read_history() {
	ece150::open_file();

	while (ece150::next_trans_entry()) {
		Transaction *new_transaction {new Transaction {
			ece150::get_trans_symbol(),
			ece150::get_trans_day(),
			ece150::get_trans_month(),
			ece150::get_trans_year(),
			ece150::get_trans_type(),
			ece150::get_trans_shares(),
			ece150::get_trans_amount()
		}};
		History::insert(new_transaction);
	}
	ece150::close_file();
}

// insert(...): Insert transaction into linked list.
//
void History::insert(Transaction *p_new_trans) {
	if (p_head == nullptr) {
		p_head = p_new_trans;
	} else {
		Transaction *current{p_head};
		while (current->get_next() != nullptr) {
			current = current->get_next();
		}
		current->set_next(p_new_trans);
	}
}


// TASK
// sort_by_date(): Sort the linked list by trade date.
//
void History::sort_by_date() {
	/*Transaction *current{p_head};
	Transaction *previous{nullptr};
	Transaction *next{nullptr};
	Transaction *temp{nullptr};
	bool not_sorted{true};

	while (not_sorted) {
		not_sorted = false;
		while(current->get_next() != nullptr) {
			next = current->get_next();

			temp = p_head;
			while (temp != current) {
				previous = temp;
				temp = temp->get_next();
			}
			if (*next < *current) { //If entries need swapping
				not_sorted = true;
				if (current == p_head) { // Beginning of list
					current->set_next(next->get_next());
					next->set_next(current);
					p_head = next;
				} else { //Everywhere else
					current->set_next(next->get_next());
					next->set_next(current);
					previous->set_next(next);
				}
			} else {
				current = current->get_next();
			}
		}
		current = p_head;
	}*/
	///////////////////////////////////////////////////////////
	Transaction *start{p_head};
	Transaction *start_previous{nullptr};
	Transaction *current{p_head};
	Transaction *previous{nullptr};
	Transaction *least{nullptr};
	Transaction *temp{nullptr};
	bool swapped{false};

	while (start != nullptr) {
		least = start;
		current = start;
		while (current != nullptr) {
			if (*current < *least) {
				swapped = true;
				temp = start;
				while (temp != current) {
					previous = temp;
					temp = temp->get_next();
				}
				least = current;
			}
			current = current->get_next();
		}
		if (swapped) {
			previous->set_next(least->get_next());
			least->set_next(start);
			if (start == p_head) {
				p_head = least;
			} else {
				temp = p_head;
				while (temp != start) {
					start_previous = temp;
					temp = temp->get_next();
				}
				start_previous->set_next(least);
			}
		} else {
			start = start->get_next();
		}
		swapped = false;
	}
}


// TASK
// update_acb_cgl(): Updates the ACB and CGL values.
//
void History::update_acb_cgl() {
	double acb{0};
	unsigned int share_balance{0};
	double acb_per_share{0};

	Transaction *current{p_head};
	while (current != nullptr) {
		if (current->get_trans_type() == 1) {
			acb += current->get_amount();
			share_balance += current->get_shares();
			acb_per_share = acb/share_balance;
		} else {
			share_balance -= current->get_shares();
			acb = acb - ((current->get_shares())*acb_per_share);
			current->set_cgl(current->get_amount() - (current->get_shares())*acb_per_share);
		}
		current->set_acb(acb);
		current->set_share_balance(share_balance);
		current->set_acb_per_share(acb_per_share);
		current = current->get_next();
	}
}


// TASK
// compute_cgl(): )Compute the ACB, and CGL.
//
double History::compute_cgl(unsigned int year) {
	double cgl{0};
	Transaction *current{p_head};

	while (current != nullptr) {
		if (current->get_year() == year) {
			cgl += current->get_cgl();
		}
		current = current->get_next();
	}
	return cgl;
}



// TASK
// print() Print the transaction history.
//
void History::print() {
	Transaction *current{p_head};
	std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
	while (current != nullptr) {
		//if (current->get_next() != nullptr) {std::cout << current->get_next()->get_symbol() << " ";}
		current->print();
		current = current->get_next();
	}
	std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
}



// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
