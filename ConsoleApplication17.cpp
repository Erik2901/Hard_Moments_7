#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <optional>
#include <vector>
#include <numeric>
#include <functional>
#include <algorithm>
#include "student.pb.h"

namespace Std_grp {
	class IRepository {
		virtual void Open() = 0;
		virtual void Save() = 0;
	};

	class IMethods {
		virtual double GetAverageScore(const FullName& name) = 0;
		virtual std::string GetAllInfo(const FullName& name) = 0;
		virtual std::string GetAllInfo() = 0;
	};

	class StudentsGroup : public IRepository, public IMethods {
	private:
		std::vector<Student> sty_lst;
	public:
		void add_student(Student &st1) {
			sty_lst.push_back(st1);
		}
		virtual double GetAverageScore(const FullName& name) {
			auto it = std::find_if(sty_lst.begin(), sty_lst.end(), [&](const Student& q)
				{return tie(q.sty_name().name(), q.sty_name().surname(), q.sty_name().patronymic()) ==
				tie(name.name(), name.surname(), name.patronymic()); });
			return it->middle_rat();
		}
		virtual std::string GetAllInfo(const FullName& name) {
			auto it = std::find_if(sty_lst.begin(), sty_lst.end(), [&](const Student& q)
				{return tie(q.sty_name().name(), q.sty_name().surname(), q.sty_name().patronymic()) ==
				tie(name.name(), name.surname(), name.patronymic()); });
			std::string info;
			std::string balls;
			for (size_t i = 0; i < it->ball_size(); i++)
			{
				balls += std::to_string(it->ball(i)) + " ";
			}
			info = "Name: " + it->sty_name().name() + '\n' + "Surname: " + it->sty_name().surname() + '\n' + "Patronymic: " + it->sty_name().patronymic() + '\n' + "Balls: " + balls + std::to_string(it->middle_rat());
			return info;
		}
		virtual std::string GetAllInfo() {
			auto it = sty_lst.begin();
			std::string info;
			for (; it < sty_lst.end(); it++)
			{
				std::string balls = " ";
				for (size_t i = 0; i < it->ball_size(); i++)
				{
					balls += std::to_string(it->ball(i)) + " ";
				}
				info = "Name: " + it->sty_name().name() + '\n' + "Surname: " + it->sty_name().surname() + '\n' + "Patronymic: " + it->sty_name().patronymic() + '\n' + "Balls: " + balls + '\n' + "Middle Ball: " + std::to_string(it->middle_rat()) + '\n';
			}
			return info;
		}
		virtual void Save() {
			std::ofstream out("Students.bin", std::ios_base::binary);
			std::for_each(sty_lst.begin(), sty_lst.end(), [&](Student& q) {q.SerializePartialToOstream(&out); });
			out.close();
		}
		virtual void Open() {
			std::ifstream in("Students.bin", std::ios_base::binary);
			Student q;
			while (!in.eof()) {
				if (q.ParseFromIstream(&in))
				{
					sty_lst.push_back(q);
				}
				else {
					std::cout << "Error";
				}
				in.close();
			}
		}

		virtual ~StudentsGroup() {};
	};
}

int main()
{
	FullName full_name;
	Student student1;
	Std_grp::StudentsGroup students_group_obj;
	full_name.set_name("Harut");
	full_name.set_surname("Poghosyan");
	full_name.set_patronymic("Hayki");

	*student1.mutable_sty_name() = full_name;
	student1.add_ball(5);
	student1.add_ball(5);
	student1.add_ball(4);
	student1.add_ball(3);
	student1.add_ball(4);
	student1.set_middle_rat(double(std::accumulate(student1.ball().begin(), student1.ball().end(), 0)) / student1.ball_size());


	students_group_obj.add_student(student1);
	std::cout << students_group_obj.GetAllInfo();
	students_group_obj.Save();

	Std_grp::StudentsGroup students_group_obj2;
	students_group_obj2.Open();
	std::cout << students_group_obj2.GetAllInfo() << std::endl;

	return 0;
}

