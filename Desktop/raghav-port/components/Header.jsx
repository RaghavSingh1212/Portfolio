import Link from "next/link";
import { Button } from "./ui/button";

// components
import Nav from "./Nav";

const Header = () => {
  return (
    <header className="py-8 xl:py-12 text-white">
      <div className="container mx-auto flex justify-between items-center">
        {/* logo */}
        <Link href="#about">
          <h1 className="text-4xl font-semibold cursor-pointer">
            Raghav<span className="text-accent"></span>
          </h1>
        </Link>

        {/* desktop nav & hire me button */}
        <div className="hidden xl:flex items-center gap-8">
          <Nav />

        </div>

        {/* mobile nav */}
        <div className="xl:hidden">
        </div>
      </div>
    </header>
  );
};

export default Header;
