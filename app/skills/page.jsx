"use client"; // For client-side rendering

import Image from "next/image";

const skillsData = [
  {
    category: "Frontend Development",
    skills: [
      { name: "TypeScript", level: "Intermediate" },
      { name: "JavaScript", level: "Intermediate" },
      { name: "HTML", level: "Intermediate" },
      { name: "CSS", level: "Intermediate" },
      { name: "Angular", level: "Intermediate" },
      { name: "RxJs", level: "Beginner" },
      { name: "React", level: "Beginner" },
    ],
  },
  {
    category: "Backend Development",
    skills: [
      { name: "Git", level: "Experienced" },
      { name: "NoSQL", level: "Beginner" },
      { name: "Node.js", level: "Beginner" },
      { name: "Firebase", level: "Beginner" },
    ],
  },
];

const Skills = () => {
  return (
    <section id="skills" className="py-16 bg-gray-100 bg-opacity-0 mb-44">
      <div className="container mx-auto px-6">
        <h1 className="text-4xl font-bold text-center mb-12 text-black/70">My Skills</h1>
        <div className="grid grid-cols-1 md:grid-cols-2 gap-12">
          {skillsData.map((skillCategory, index) => (
            <div
              key={index}
              className="bg-transparent p-6 rounded-lg shadow-lg border border-black/30 "
            >
              <h2 className="text-2xl font-semibold mb-6 text-black/70">{skillCategory.category}</h2>
              <div className="grid grid-cols-2 gap-6">
                {skillCategory.skills.map((skill, index) => (
                  <div key={index} className="flex items-center space-x-4">
                    <Image
                      src="/assets/checkmark.png" // Icon for the skill
                      alt="Experience icon"
                      width={24}
                      height={24}
                      className="object-contain"
                    />
                    <div>
                      <h3 className="text-lg font-semibold text-black/70">{skill.name}</h3>
                      <p className="text-white">{skill.level}</p>
                    </div>
                  </div>
                ))}
              </div>
            </div>
          ))}
        </div>
      </div>
    </section>
  );
};

export default Skills;
