#ifndef _GENOME_H
#define _GENOME_H

/////////////////////////////////////////////////////////////////
// NEAT
// --------------------------------------------------------------
// NeuroEvolution of Augmenting Topologies C++ implementation
//
//  
//
// Peter Chervenski
////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// File:        Genome.h
// Description: Definition for the Genome class.
///////////////////////////////////////////////////////////////////////////////

#include <vector>

#include "NeuralNetwork.h"
#include "Substrate.h"
#include "Innovation.h"
#include "Genes.h"
#include "Assert.h"
#include "PhenotypeBehavior.h"

namespace NEAT
{


//////////////////////////////////////////////
// The Genome class
//////////////////////////////////////////////

// forward
class Innovation;
class InnovationDatabase;
class PhenotypeBehavior;

class Genome
{
    /////////////////////
    // Members
    /////////////////////
private:
    // ID of genome
    unsigned int m_ID;

    // The two lists of genes
    std::vector<NeuronGene> m_NeuronGenes;
    std::vector<LinkGene>   m_LinkGenes;

    // How many inputs/outputs
    unsigned int m_NumInputs;
    unsigned int m_NumOutputs;

    // The genome's fitness score
    double m_Fitness;

    // The genome's adjusted fitness score
    double m_AdjustedFitness;

    // The depth of the network
    unsigned int m_Depth;

    // how many individuals this genome should spawn
    double m_OffspringAmount;

    // The species ID that this genome belogs to
    //unsigned int m_BelongsToSpeciesID;

    ////////////////////
    // Private methods

    // Returns true if the specified neuron ID is present in the genome
    bool HasNeuronID(unsigned int a_id) const;

    // Returns true if the specified link is present in the genome
    bool HasLink(unsigned int a_n1id, unsigned int a_n2id) const;

    // Returns true if the specified link is present in the genome
    bool HasLinkByInnovID(unsigned int a_id) const;

    // Removes the link with the specified innovation ID
    void RemoveLinkGene(unsigned int a_innovid);

    // Remove node
    // Links connected to this node are also removed
    void RemoveNeuronGene(unsigned int a_id);

    // Returns the count of links inputting from the specified neuron ID
    int LinksInputtingFrom(unsigned int a_id) const;

    // Returns the count of links outputting to the specified neuron ID
    int LinksOutputtingTo(unsigned int a_id) const;

    // A recursive function returning the max depth from the specified neuron to the inputs
    unsigned int NeuronDepth(unsigned int a_NeuronID, unsigned int a_Depth);

    // Returns true is the specified neuron ID is a dead end or isolated
    bool IsDeadEndNeuron(unsigned int a_id) const;

public:

    // tells whether this genome was evaluated already
    // used in steady state evolution
    bool m_Evaluated;

    // A pointer to a class representing the phenotype's behavior
    // Used in novelty searches
    PhenotypeBehavior* m_PhenotypeBehavior;

    ////////////////////////////
    // Constructors
    ////////////////////////////

    Genome();

    // copy constructor
    Genome(const Genome& a_g);

    // assignment operator
    Genome& operator=(const Genome& a_g);

    // Builds this genome from a file
    Genome(char* a_filename);

    // Builds this genome from an opened file
    Genome(std::ifstream& a_DataFile);

    // This creates a standart minimal genome - perceptron-like structure
    Genome(unsigned int a_ID,
           unsigned int a_NumInputs,
           unsigned int a_NumHidden, // ignored for type == 1, specifies number of hidden units if type == 0
           unsigned int a_NumOutputs,
           bool a_FS_NEAT, ActivationFunction a_OutputActType, ActivationFunction a_HiddenActType, unsigned int a_SeedType);

    /////////////
    // Other possible constructors for different types of networks go here
    // TODO
    /////////////

    ////////////////////////////
    // Destructor
    ////////////////////////////

    ////////////////////////////
    // Methods
    ////////////////////////////

    ////////////////////
    // Accessor methods

    NeuronGene GetNeuronByID(unsigned int a_ID) const
    {
        ASSERT(HasNeuronID(a_ID));
        int t_idx = GetNeuronIndex(a_ID);
        ASSERT(t_idx != -1);
        return m_NeuronGenes[t_idx];
    }

    NeuronGene GetNeuronByIndex(unsigned int a_idx) const
    {
        ASSERT(a_idx < m_NeuronGenes.size());
        return m_NeuronGenes[a_idx];
    }

    LinkGene GetLinkByInnovID(unsigned int a_ID) const
    {
        ASSERT(HasLinkByInnovID(a_ID));
        for(unsigned int i=0; i<m_LinkGenes.size(); i++)
            if (m_LinkGenes[i].InnovationID() == a_ID)
                return m_LinkGenes[i];

        // should never reach this code
        throw std::exception();
    }

    LinkGene GetLinkByIndex(unsigned int a_idx) const
    {
        ASSERT(a_idx < m_LinkGenes.size());
        return m_LinkGenes[a_idx];
    }

    // A little helper function to find the index of a neuron, given its ID
    unsigned int GetNeuronIndex(unsigned int a_id) const;

    // A little helper function to find the index of a link, given its innovation ID
    unsigned int GetLinkIndex(unsigned int a_innovid) const;

    unsigned int NumNeurons() const { return static_cast<unsigned int>(m_NeuronGenes.size()); }
    unsigned int NumLinks() const { return static_cast<unsigned int>(m_LinkGenes.size()); }
    unsigned int NumInputs() const { return m_NumInputs; }
    unsigned int NumOutputs() const { return m_NumOutputs; }

    void SetNeuronXY(unsigned int a_idx, int a_x, int a_y)
    {
        ASSERT(a_idx < m_NeuronGenes.size());
        m_NeuronGenes[a_idx].x = a_x;
        m_NeuronGenes[a_idx].y = a_y;
    }
    void SetNeuronX(unsigned int a_idx, int a_x)
    {
        ASSERT(a_idx < m_NeuronGenes.size());
        m_NeuronGenes[a_idx].x = a_x;
    }
    void SetNeuronY(unsigned int a_idx, int a_y)
    {
        ASSERT(a_idx < m_NeuronGenes.size());
        m_NeuronGenes[a_idx].y = a_y;
    }


    double GetFitness() const { return m_Fitness; }
    double GetAdjFitness() const { return m_AdjustedFitness; }
    void SetFitness(double a_f) { m_Fitness = a_f; }
    void SetAdjFitness(double a_af) { m_AdjustedFitness = a_af; }

    unsigned int GetID() const { return m_ID; }
    void SetID(int a_id) { m_ID = a_id; }

    unsigned int GetDepth() const { return m_Depth; }
    void SetDepth(int a_d) { m_Depth = a_d; }

    // Returns true if there is any dead end in the network
    bool HasDeadEnds() const;

    double GetOffspringAmount() const { return m_OffspringAmount; }
    void SetOffspringAmount(double a_oa) { m_OffspringAmount = a_oa; }

    // This builds a fastnetwork structure out from the genome
    void BuildPhenotype(NeuralNetwork& net) const;

    // Projects the phenotype's weights back to the genome
    void DerivePhenotypicChanges(NeuralNetwork& a_Net);


    ////////////
    // Other possible methods for building a phenotype go here
    // Like CPPN/HyperNEAT stuff
    ////////////
    void BuildHyperNEATPhenotype(NeuralNetwork& net, Substrate* subst,double a_LinkTreshold, double a_MaxWeight, double a_MinTimeConst, double a_MaxTimeConst);

    // Saves this genome to a file
    void Save(char* a_filename) const;

    // Saves this genome to an already opened file for writing
    void Save(FILE* a_fstream) const;

    // returns the max neuron ID
    unsigned int GetLastNeuronID() const;

    // returns the max innovation Id
    unsigned int GetLastInnovationID() const;

    // Sorts the genes of the genome
    // The neurons by IDs and the links by innovation numbers.
    void SortGenes();

    // overload '<' used for sorting. From fittest to poorest.
    friend bool operator<(const Genome& a_lhs, const Genome& a_rhs)
    {
        return (a_lhs.m_Fitness > a_rhs.m_Fitness);
    }

    // Returns true if this genome and a_G are compatible (belong in the same species)
    bool IsCompatibleWith(Genome& a_G);

    // returns the absolute compatibility distance between this genome and a_G
    double CompatibilityDistance(Genome &a_G);




    // Calculates the network depth
    void CalculateDepth();

    ////////////
    // Mutation
    ////////////

    // Adds a new neuron to the genome
    // returns true if succesful
    bool Mutate_AddNeuron(InnovationDatabase &a_Innovs);

    // Adds a new link to the genome
    // returns true if succesful
    bool Mutate_AddLink(InnovationDatabase &a_Innovs);

    // Remove a random link from the genome
    // A cleanup procedure is invoked so any dead-ends or stranded neurons are also deleted
    // returns true if succesful
    bool Mutate_RemoveLink();

    // Removes a hidden neuron having only one input and only one output with
    // a direct link between them.
    bool Mutate_RemoveSimpleNeuron(InnovationDatabase& a_Innovs);

    // Perturbs the weights
    void Mutate_LinkWeights();

    // Set all link weights to random values between [-R .. R]
    void Randomize_LinkWeights(double a_Range);

    // Perturbs the A parameters of the neuron activation functions
    void Mutate_NeuronActivations_A();

    // Perturbs the B parameters of the neuron activation functions
    void Mutate_NeuronActivations_B();

    // Changes the activation function type for a random neuron
    void Mutate_NeuronActivation_Type();

    // Perturbs the neuron time constants
    void Mutate_NeuronTimeConstants();

    // Perturbs the neuron biases
    void Mutate_NeuronBiases();


    ///////////
    // Mating
    ///////////


    // Mate this genome with dad and return the baby
    // This is multipoint mating - genes inherited randomly
    // If the bool is true, then the genes are averaged
    // Disjoint and excess genes are inherited from the fittest parent
    // If fitness is equal, the smaller genome is assumed to be the better one
    Genome Mate(Genome& a_dad, bool a_averagemating, bool a_interspecies);


    //////////
    // Utility
    //////////


    // Checks for the genome's integrity
    // returns false if something is wrong
    bool Verify() const;

    // Search the genome for isolated structure and clean it up
    // Returns true is something was removed
    bool Cleanup();




    ////////////////////
    // new stuff

    bool m_Parent; // a marker that tells this genome is used already and should be deleted
    void Birth() { m_Parent = false;  }   // called for every new born baby
    void Adult() { m_Parent = true; }   // called before reproduction
    bool IsBaby()  const { return !m_Parent; }
    bool IsAdult() const { return m_Parent; }

    bool IsEvaluated() const { return m_Evaluated; }
    void SetEvaluated() { m_Evaluated = true; }
    void ResetEvaluated() { m_Evaluated = false; }
};







} // namespace NEAT

#endif
