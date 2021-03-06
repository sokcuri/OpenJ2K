#pragma once

struct j2k_base_dict
{
	j2k_base_dict() {};
	j2k_base_dict(
		std::int32_t id,
		std::string kor,
		std::string pumsa,
		std::string prop1,
		std::string prop2,
		std::string dict,
		std::int32_t next,
		int extra,
		int32_t timestamp
	) :
		id(id),
		kor(kor),
		pumsa(pumsa),
		prop1(prop1),
		prop2(prop2),
		dict(dict),
		next(next),
		extra(extra),
		timestamp(timestamp) { }

	std::int32_t id;
	std::string kor;
	std::string pumsa;
	std::string prop1;
	std::string prop2;
	std::string dict;
	std::int32_t next;
	int extra;
	int32_t timestamp;

	template <typename Archive, typename Self>
	static void serialize(Archive& archive, Self& self)
	{
		archive(self.id, self.kor, self.pumsa, self.prop1, self.prop2, self.dict, self.next, self.extra, self.timestamp);
	}
};
