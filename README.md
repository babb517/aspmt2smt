%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% aspmt2smt                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Author - Joseph Babb  (jbabb1 -at- asu -dot- edu)                                     %
% Date - Spring 2013                                                                    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

aspmt2smt is an automated system for solving propositional ASPMT programs via a reduction
to SMT. ASPMT follows the translation defined by Bartholomew and Lee 2013 [1] by
performing a 3 step process:

      1) Convert the ASPMT program P into Clark Normal Form, denoted CNF(P).
      2) Calculate the completion of CNF(P) (COMP[CNF(P)]).
      3) Perform a variable elimination procedure to obtain a ground SMT program.
      
The program then uses an existing SMT solver (such as the Microsoft Z3 solver) in order
to compute the models of the Z3 program, displaying the results to the user.

References:

[1]   Bartholomew and Lee. Functional Stable Model Semantics and Answer Set Programming
      Modulo Theories. Submitted to IJCAI 2013. Pending Review.
      
[2]   Bartholomew and Lee. Stable Models of Formulas with Intensional Functions.
      In proceedings of the International Conference on Principals of Knowledge
      Representation and Reasoning. Pages 2-12, 2012.
