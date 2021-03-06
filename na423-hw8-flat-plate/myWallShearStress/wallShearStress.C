/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2010 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    wallShearStress

Description
    Calculates and writes the wall shear stress, for the specified times.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "incompressible/singlePhaseTransportModel/singlePhaseTransportModel.H"
#include "RASModel.H"

#include "OFstream.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    timeSelector::addOptions();
    #include "setRootCase.H"
    #include "createTime.H"
    instantList timeDirs = timeSelector::select0(runTime, args);
    #include "createMesh.H"

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);
        Info<< "Time = " << runTime.timeName() << endl;
        mesh.readUpdate();

        #include "createFields.H"

        volSymmTensorField Reff(RASModel->devReff());

        volVectorField wallShearStress
        (
            IOobject
            (
                "wallShearStress",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            mesh,
            dimensionedVector
            (
                "wallShearStress",
                Reff.dimensions(),
                vector::zero
            )
        );

        forAll(wallShearStress.boundaryField(), patchi)
        {
            wallShearStress.boundaryField()[patchi] =
            (
                -mesh.Sf().boundaryField()[patchi]
                /mesh.magSf().boundaryField()[patchi]
            ) & Reff.boundaryField()[patchi];
        }

        wallShearStress.write();

// write out the x component of the wall shear stress

        const fvPatchList& patches = mesh.boundary();

        label patchID = mesh.boundaryMesh().findPatchID("plate");

        const fvPatch& curPatch = patches[patchID];

        OFstream wallStress("wallStress.dat");
        wallStress << "#x    tw/rho   yPlus*nu " << endl;

        forAll(curPatch, facei)
        {
            vector Cfi = curPatch.Cf()[facei];
            scalar deltaCo  = curPatch.deltaCoeffs()[facei];
            const vector& wallSheari =
                   wallShearStress.boundaryField()[patchID][facei];

            scalar Cfx = Cfi.x();
            scalar taux = wallSheari.x();

            wallStress << Cfx  << " " << -taux << " " 
                       << 1.0/deltaCo*Foam::sqrt(mag(taux)) << endl;
        }


    }

    Info<< "End" << endl;

    return 0;
}


// ************************************************************************* //
