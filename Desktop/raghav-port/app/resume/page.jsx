"use client";
import { motion } from "framer-motion";

const educationData = [
  {
    year: "2022 - 2026",
    title: "Computer Science - University of California Santa Cruz",
    description:
      "GPA: 3.80/4.0 ; \n\n Relevant Coursework: Data Structures & Algorithms, Operating Systems, Computer Architecture, Distributed Systems, Artificial Intelligence, Machine Learning, Software Design, System Design, Cloud Computing, Abstraction, Python, Assembly Language.",
  },
];

const experienceData = [
  {
    year: "Jan 2025 - Present",
    title: "Software Engineer Co-Op Intern - Nutanix",
    description:
      "Optimizing GenAI inferencing with SIMD instructions and enhancing performance with Prometheus/Grafana dashboards. Exploring Retrieval-Augmented Generation to improve AI pipelines.",
    logo: "/assets/nut.png",  // Add the specific logo for Nutanix
  },
  {
    year: "Sep 2024 - Present",
    title: "Machine Learning Researcher - AIEA Lab",
    description:
      "Improved Transformer models by reducing hallucination errors and integrated OpenAIs API with SwiProlog for efficient translation of natural language to executable Prolog facts.",
    logo: "/assets/aiea.png",  // Add the specific logo for AIEA Lab
  },
  {
    year: "Jan 2024 - Sep 2024",
    title: "Software Engineer Intern - Tech4Good",
    description:
      "Developed UI components for a goal-setting app using Angular and optimized performance with NgRx and RxJS. Created responsive UI components to improve cross-platform compatibility.",
    logo: "/assets/t4.png",  // Add the specific logo for Tech4Good
  },
  {
    year: "Sep 2023 - Dec 2024",
    title: "Group Tutor - Baskin School of Engineering",
    description:
      "Tutored CSE20, CSE30, CSE12, CSE16, CSE101",
    logo: "/assets/jack.png",  // Add the specific logo for UCSC
  },
  {
    year: "Mar 2024 - Jun 2024",
    title: "Web Developer - UCSC Blueprint",
    description:
      "Developed a career matching platform using React and Firebase, aligning 500+ student assessments with job opportunities for personalized career pathways.",
    logo: "/assets/blue.png",  // Add the specific logo for UCSC Blueprint
  },
];


const EducationExperience = () => {
  return (
    <section id="education" className="py-16">
      <div className="container mx-auto px-6">
        <h2 className="text-4xl font-bold text-center mb-12">My Journey</h2>

        <div className="flex flex-col md:flex-row gap-12">
          {/* Education Section */}
          {/* Education Section */}
<motion.div
  className="w-full md:w-1/2"
  initial={{ opacity: 0, x: -50 }}
  whileInView={{ opacity: 1, x: 0 }}
  transition={{ duration: 0.6 }}
  viewport={{ once: true }}
>
  <h3 className="text-2xl font-semibold mb-6">Education</h3>
  <div className="border-l-4 border-black/30 pl-4 space-y-6">
    {educationData.map((edu, index) => (
      <motion.div
        key={index}
        className="p-6 rounded-lg shadow-lg relative border border-black/30"
        whileHover={{ scale: 1.05 }}
      >
        {/* Year inside the box at the top */}
        <div className="absolute top-0 left-1/2 -translate-x-1/2 -translate-y-1/2 bg-black/70 text-white/80 px-4 py-1 rounded-full text-sm font-bold shadow-md">
          {edu.year}
        </div>
        
        {/* Add small logo before title */}
        <div className="flex items-center mt-6 space-x-3">
          <img
            src="/assets/ucsc.png"
            alt="UC Santa Cruz Logo"
            className="w-6 h-6 object-contain"
          />
          <h3 className="text-lg font-bold">{edu.title}</h3>
        </div>

        <p className="text-gray-600 whitespace-pre-line">{edu.description}</p>
      </motion.div>
    ))}
  </div>
</motion.div>


          {/* Experience Section (Now Scrollable) */}
{/* Experience Section (Now Scrollable) */}
<motion.div
  className="w-full md:w-1/2"
  initial={{ opacity: 0, x: 50 }}
  whileInView={{ opacity: 1, x: 0 }}
  transition={{ duration: 0.6 }}
  viewport={{ once: true }}
>
  <h3 className="text-2xl font-semibold mb-6">Experience</h3>
  <div className="border-l-4 border-black/30 pl-4 space-y-6 max-h-[600px] overflow-y-auto scrollbar-thin scrollbar-thumb-green-500 scrollbar-track-gray-100 pt-2">
    {experienceData.map((exp, index) => (
      <motion.div
        key={index}
        className="p-6 pt-10 rounded-lg shadow-lg relative border border-black/30"
        whileHover={{ scale: 1.05 }}
      >
        {/* Year inside the box at the top */}
        <div className="absolute top-0 left-1/2 -translate-x-1/2 -translate-y-1/2 bg-black/70 text-white/80 px-4 py-1 rounded-full text-sm font-bold shadow-md">
          {exp.year}
        </div>

        {/* Add small logo before title */}
        <div className="flex items-center mt-6 space-x-3">
          <img
            src={exp.logo}  // Dynamically add the logo
            alt={`${exp.title} Logo`}  // Alt text for better accessibility
            className="w-6 h-6 object-contain"
          />
          <h3 className="text-lg font-bold">{exp.title}</h3>
        </div>

        <p className="text-gray-600">{exp.description}</p>
      </motion.div>
    ))}
  </div>
</motion.div>



        </div>
      </div>
    </section>
  );
};

export default EducationExperience;
